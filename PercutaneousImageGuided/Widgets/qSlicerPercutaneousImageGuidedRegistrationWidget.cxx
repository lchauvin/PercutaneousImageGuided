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

// Registration Widgets includes
#include "qSlicerPercutaneousImageGuidedRegistrationWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedRegistrationWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

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




