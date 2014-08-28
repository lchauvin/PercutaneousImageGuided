/*==============================================================================

  Program: 3D Slicer

  Copyright (c) Kitware Inc.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

  This file was originally developed by Jean-Christophe Fillion-Robin, Kitware Inc.
  and was partially funded by NIH grant 3P41RR013218-12S1

==============================================================================*/

#include "qSlicerCoreApplication.h"
#include "qSlicerCorePythonManager.h"

#include "vtkMRMLScalarVolumeNode.h"
#include "vtkMRMLMarkupsFiducialNode.h"
#include "vtkMRMLRobotModelNode.h"

// Registration Widgets includes
#include "qSlicerPercutaneousImageGuidedRegistrationWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedRegistrationWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

#include <QMessageBox>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedRegistrationWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedRegistrationWidget);
protected:
  qSlicerPercutaneousImageGuidedRegistrationWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate(
    qSlicerPercutaneousImageGuidedRegistrationWidget& object);
  ~qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate();
  virtual void setupUi(qSlicerPercutaneousImageGuidedRegistrationWidget*);

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate
::qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate(
  qSlicerPercutaneousImageGuidedRegistrationWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate
::~qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate()
{
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedRegistrationWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedRegistrationWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedRegistrationWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRegistrationWidget
::qSlicerPercutaneousImageGuidedRegistrationWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPercutaneousImageGuidedRegistrationWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedRegistrationWidget);
  d->setupUi(this);

  connect(d->RegisterRobotButton, SIGNAL(clicked()),
	  this, SLOT(onRegisterButtonClicked()));
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRegistrationWidget
::~qSlicerPercutaneousImageGuidedRegistrationWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRegistrationWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedRegistrationWidget);

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->activeParamNode;
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent,
			 this, SLOT(refreshWidget()));
    }

  d->activeParamNode = activeNode;
  this->refreshWidget();

  this->qvtkConnect(d->activeParamNode, vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent,
		    this, SLOT(refreshWidget()));
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedRegistrationWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedRegistrationWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRegistrationWidget::refreshWidget()
{
  std::cerr << "Udpate Registration Widget" << std::endl;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRegistrationWidget::onRegisterButtonClicked()
{
  Q_D(qSlicerPercutaneousImageGuidedRegistrationWidget);

  vtkMRMLScalarVolumeNode* selectedVolume = 
    vtkMRMLScalarVolumeNode::SafeDownCast(d->RegistrationImageSelector->currentNode());

  vtkMRMLMarkupsFiducialNode* selectedFiducials =
    vtkMRMLMarkupsFiducialNode::SafeDownCast(d->RegistrationFiducialListSelector->currentNode());

  vtkSmartPointer<vtkMRMLLinearTransformNode> registrationTransform =
    vtkSmartPointer<vtkMRMLLinearTransformNode>::New();

  if (selectedVolume && selectedFiducials && registrationTransform)
    {
    this->mrmlScene()->AddNode(registrationTransform);

    qSlicerCorePythonManager* pythonManager = qSlicerCoreApplication::application()->corePythonManager();
    if (pythonManager)
      {
      // Run registration module FiducialToImageRegistration (python)
      QString pythonScript("reg = FiducialToImageRegistrationLogic();"
			   "ret = reg.run('%1','%2','%3')");
      pythonScript = pythonScript.arg(selectedVolume->GetID(), 
				      selectedFiducials->GetID(), 
				      registrationTransform->GetID());

      pythonManager->executeString(pythonScript);
      QVariant registrationSucceed = pythonManager->getVariable("ret");

      if (registrationSucceed.toBool())
	{
	// Register robot model node
	if (d->activeParamNode && this->getActiveParameterNode()->GetRobotModelNodeID())
	  {
	  vtkMRMLRobotModelNode* robotModelNode = 
	    vtkMRMLRobotModelNode::SafeDownCast(this->mrmlScene()->GetNodeByID(this->getActiveParameterNode()->GetRobotModelNodeID()));
	  
	  if (robotModelNode)
	    {
	    robotModelNode->SetAndObserveTransformNodeID(registrationTransform->GetID());
	    }
	  }
	
	// Update registration node ID
	d->activeParamNode->SetAndObserveRegistrationNodeID(registrationTransform->GetID());

	QMessageBox::information(this,"Registration","Registration Succeed");
	}
      else
	{
	QMessageBox::critical(this,"Registration","Registration Failed");
	}
      }
    }
}




