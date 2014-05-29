/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#include <sstream>

// Qt includes
#include <QDebug>
#include <QProcess>

// SlicerQt includes
#include "qSlicerPercutaneousImageGuidedModuleWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedModuleWidget.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"
#include "vtkMRMLScene.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPercutaneousImageGuidedModuleWidgetPrivate: public Ui_qSlicerPercutaneousImageGuidedModuleWidget
{
public:
  qSlicerPercutaneousImageGuidedModuleWidgetPrivate();
  ~qSlicerPercutaneousImageGuidedModuleWidgetPrivate();

  void setPreRegParamNode(vtkMRMLPercutaneousImageGuidedParameterNode* preReg) { this->PreRegParamNode = preReg; } 
  vtkMRMLPercutaneousImageGuidedParameterNode* getPreRegParamNode() { return this->PreRegParamNode; }

  void setRegParamNode(vtkMRMLPercutaneousImageGuidedParameterNode* reg) { this->RegParamNode = reg; } 
  vtkMRMLPercutaneousImageGuidedParameterNode* getRegParamNode() { return this->RegParamNode; }

  void setPostRegParamNode(vtkMRMLPercutaneousImageGuidedParameterNode* postReg) { this->PostRegParamNode = postReg; } 
  vtkMRMLPercutaneousImageGuidedParameterNode* getPostRegParamNode() { return this->PostRegParamNode; }

  void setPostInsParamNode(vtkMRMLPercutaneousImageGuidedParameterNode* postIns) { this->PostInsParamNode = postIns; }
  vtkMRMLPercutaneousImageGuidedParameterNode* getPostInsParamNode() { return this->PostInsParamNode; }

  void setActiveParamNode(vtkMRMLPercutaneousImageGuidedParameterNode* active) { this->ActiveParamNode = active; } 
  vtkMRMLPercutaneousImageGuidedParameterNode* getActiveParamNode() { return this->ActiveParamNode; }

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* PreRegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* RegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* PostRegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* PostInsParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* ActiveParamNode;
};	      
	      
//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidgetPrivate::qSlicerPercutaneousImageGuidedModuleWidgetPrivate()
{
  this->PreRegParamNode	 = NULL;
  this->RegParamNode	 = NULL;
  this->PostRegParamNode = NULL;
  this->PostInsParamNode = NULL;
  this->ActiveParamNode  = NULL;
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidgetPrivate::~qSlicerPercutaneousImageGuidedModuleWidgetPrivate()
{
  if (this->PreRegParamNode)
    {
    this->PreRegParamNode->Delete();
    }

  if (this->RegParamNode)
    {
    this->RegParamNode->Delete();
    }

  if (this->PostRegParamNode)
    {
    this->PostRegParamNode->Delete();
    }

  if (this->PostInsParamNode)
    {
    this->PostInsParamNode->Delete();
    }
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedModuleWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidget::qSlicerPercutaneousImageGuidedModuleWidget(QWidget* _parent)
  : Superclass( _parent )
  , d_ptr( new qSlicerPercutaneousImageGuidedModuleWidgetPrivate )
{
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidget::~qSlicerPercutaneousImageGuidedModuleWidget()
{
  this->qvtkDisconnectAll();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::setup()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);
  d->setupUi(this);
  this->Superclass::setup();

  d->tabWidget->setTabEnabled(DICOM_TAB,true);
  d->tabWidget->setTabEnabled(WORKSPACE_TAB,true);
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,false);
  d->tabWidget->setTabEnabled(ROBOT_TAB,false);
  d->tabWidget->setTabEnabled(RESLICE_TAB,false);

  // Steps
  connect(d->PreRegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPreRegButtonToggled(bool)));

  connect(d->RegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onRegButtonToggled(bool)));

  connect(d->PostRegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPostRegButtonToggled(bool)));

  connect(d->PostInsButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPostInsButtonToggled(bool)));

  // DICOM

  // Targets

  // Workspace

  // Registration
  
  // Robot

  // Reslice
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::enter()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!d->getPreRegParamNode())
    {
    vtkMRMLPercutaneousImageGuidedParameterNode* preRegNode = 
      vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
	this->mrmlScene()->CreateNodeByClass("vtkMRMLPercutaneousImageGuidedParameterNode"));
    if (preRegNode)
      {
      preRegNode->SetName("PreRegistrationParameterNode");
      d->setPreRegParamNode(preRegNode);
      this->mrmlScene()->AddNode(preRegNode);
      this->setActiveParameterNode(preRegNode);
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::exit()
{
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onPreRegButtonToggled(bool pressed)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!pressed)
    {
    return;
    }

  // Activate / Deactivate tabs
  d->tabWidget->setTabEnabled(DICOM_TAB,true);
  d->tabWidget->setTabEnabled(TARGETS_TAB,true);
  d->tabWidget->setTabEnabled(WORKSPACE_TAB,true);
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,false);
  d->tabWidget->setTabEnabled(ROBOT_TAB,false);
  d->tabWidget->setTabEnabled(RESLICE_TAB,true);

  // Create a new node if doesn't exists yet
  if (!d->getPreRegParamNode())
    {
    vtkMRMLPercutaneousImageGuidedParameterNode* preRegNode = vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLPercutaneousImageGuidedParameterNode"));
    if (preRegNode)
      {
      preRegNode->SetName("PreRegistrationParameterNode");
      d->setPreRegParamNode(preRegNode);
      this->mrmlScene()->AddNode(preRegNode);
      }
    }

  // Set Active parameter node
  this->setActiveParameterNode(d->getPreRegParamNode());
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onRegButtonToggled(bool pressed)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!pressed)
    {
    return;
    }

  // Activate / Deactivate tabs
  d->tabWidget->setTabEnabled(DICOM_TAB,true);
  d->tabWidget->setTabEnabled(TARGETS_TAB,false);
  d->tabWidget->setTabEnabled(WORKSPACE_TAB,false);
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,true);
  d->tabWidget->setTabEnabled(ROBOT_TAB,false);
  d->tabWidget->setTabEnabled(RESLICE_TAB,false);

  // Create a new node if doesn't exists yet
  if (!d->getRegParamNode())
    {
    vtkMRMLPercutaneousImageGuidedParameterNode* regNode = vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLPercutaneousImageGuidedParameterNode"));
    if (regNode)
      {
      regNode->SetName("RegistrationParameterNode");
      d->setRegParamNode(regNode);
      this->mrmlScene()->AddNode(regNode);
      }
    }

  // Set Active parameter node
  this->setActiveParameterNode(d->getRegParamNode());
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onPostRegButtonToggled(bool pressed)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!pressed)
    {
    return;
    }
  
  // Activate / Deactivate tabs
  d->tabWidget->setTabEnabled(DICOM_TAB,true);
  d->tabWidget->setTabEnabled(TARGETS_TAB,true);
  d->tabWidget->setTabEnabled(WORKSPACE_TAB,false);
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,false);
  d->tabWidget->setTabEnabled(ROBOT_TAB,true);
  d->tabWidget->setTabEnabled(RESLICE_TAB,true);

  // Create a new node if doesn't exists yet
 if (!d->getPostRegParamNode())
    {
    vtkMRMLPercutaneousImageGuidedParameterNode* postRegNode = vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLPercutaneousImageGuidedParameterNode"));
    if (postRegNode)
      {
      postRegNode->SetName("PostRegistrationParameterNode");
      d->setPostRegParamNode(postRegNode);
      this->mrmlScene()->AddNode(postRegNode);
      }
    }

  // Set Active parameter node
  this->setActiveParameterNode(d->getPostRegParamNode());
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onPostInsButtonToggled(bool pressed)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!pressed)
    {
    return;
    }
  
  // Activate / Deactivate tabs
  d->tabWidget->setTabEnabled(DICOM_TAB,true);
  d->tabWidget->setTabEnabled(TARGETS_TAB,true);
  d->tabWidget->setTabEnabled(WORKSPACE_TAB,false);
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,false);
  d->tabWidget->setTabEnabled(ROBOT_TAB,true);
  d->tabWidget->setTabEnabled(RESLICE_TAB,true);

  // Create a new node if doesn't exists yet
  if (!d->getPostInsParamNode())
    {
    vtkMRMLPercutaneousImageGuidedParameterNode* postInsNode = vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLPercutaneousImageGuidedParameterNode"));
    if (postInsNode)
      {
      postInsNode->SetName("PostInsertionParameterNode");
      d->setPostInsParamNode(postInsNode);
      this->mrmlScene()->AddNode(postInsNode);
      }
    }
  
  // Set Active parameter node
  this->setActiveParameterNode(d->getPostInsParamNode());
}


//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!activeNode)
    {
    return;
    }
  
  // Set new active parameter node
  d->setActiveParamNode(activeNode);

  if (d->DICOMWidget && d->TargetsWidget)
    {
    d->DICOMWidget->setActiveParameterNode(activeNode);
    d->TargetsWidget->setActiveParameterNode(activeNode);
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshAllWidgets()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!d->getActiveParamNode())
    {
    return;
    }
  
  this->refreshDICOMWidget();
  this->refreshTargetsWidget();
  this->refreshWorkspaceWidget();
  this->refreshRegistrationWidget();
  this->refreshRobotWidget();
  this->refreshResliceWidget();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshDICOMWidget()
{
  std::cerr << "Update DICOM Widget" << std::endl;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshTargetsWidget()
{
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshWorkspaceWidget()
{
  std::cerr << "Update Workspace Widget" << std::endl;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshRegistrationWidget()
{
  std::cerr << "Update Registration Widget" << std::endl;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshRobotWidget()
{
  std::cerr << "Update Robot Widget" << std::endl;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::refreshResliceWidget()
{
  std::cerr << "Update Reslice Widget" << std::endl;
}
