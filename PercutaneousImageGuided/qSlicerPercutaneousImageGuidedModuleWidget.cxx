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

#include "vtkMRMLIGTLConnectorNode.h"
#include "vtkMRMLIGTLSessionManagerNode.h"
#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"
#include "vtkMRMLScene.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPercutaneousImageGuidedModuleWidgetPrivate: public Ui_qSlicerPercutaneousImageGuidedModuleWidget
{
public:
  qSlicerPercutaneousImageGuidedModuleWidgetPrivate();
  ~qSlicerPercutaneousImageGuidedModuleWidgetPrivate();

  void setConnectorNode(vtkMRMLIGTLConnectorNode* cnode) { this->IGTLConnectorNode = cnode; }
  vtkMRMLIGTLConnectorNode* getConnectorNode() { return this->IGTLConnectorNode; }

  void setSessionManagerNode(vtkMRMLIGTLSessionManagerNode* snode) { this->IGTLSessionManagerNode = snode; }
  vtkMRMLIGTLSessionManagerNode* getSessionManagerNode() { return this->IGTLSessionManagerNode; }

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

  void setRobotModelNode(vtkMRMLRobotModelNode* robotNode) { this->RobotModelNode = robotNode; }
  vtkMRMLRobotModelNode* getRobotModelNode() { return this->RobotModelNode; }

private:
  vtkMRMLIGTLConnectorNode* IGTLConnectorNode;
  vtkMRMLIGTLSessionManagerNode* IGTLSessionManagerNode;

  vtkMRMLPercutaneousImageGuidedParameterNode* PreRegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* RegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* PostRegParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* PostInsParamNode;
  vtkMRMLPercutaneousImageGuidedParameterNode* ActiveParamNode;

  vtkMRMLRobotModelNode* RobotModelNode;
};	      
	      
//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidgetPrivate::qSlicerPercutaneousImageGuidedModuleWidgetPrivate()
{
  this->IGTLConnectorNode      = NULL;
  this->IGTLSessionManagerNode = NULL;

  this->PreRegParamNode	 = NULL;
  this->RegParamNode	 = NULL;
  this->PostRegParamNode = NULL;
  this->PostInsParamNode = NULL;
  this->ActiveParamNode  = NULL;

  this->RobotModelNode   = NULL;
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
  d->tabWidget->setTabEnabled(REGISTRATION_TAB,false);
  d->tabWidget->setTabEnabled(ROBOT_TAB,false);
  d->tabWidget->setTabEnabled(RESLICE_TAB,false);

  // Setup
  connect(d->ConnectButton, SIGNAL(clicked()),
	  this, SLOT(onConnectClicked()));

  connect(d->RobotModelNodeSelector, SIGNAL(currentNodeChanged(vtkMRMLNode*)),
	  this, SLOT(onRobotModelChanged(vtkMRMLNode*)));

  // Steps
  connect(d->PreRegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPreRegButtonToggled(bool)));

  connect(d->RegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onRegButtonToggled(bool)));

  connect(d->PostRegButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPostRegButtonToggled(bool)));

  connect(d->PostInsButton, SIGNAL(toggled(bool)),
	  this, SLOT(onPostInsButtonToggled(bool)));

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
void qSlicerPercutaneousImageGuidedModuleWidget::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!activeNode)
    {
    return;
    }
  
  // Set new active parameter node
  d->setActiveParamNode(activeNode);

  if (d->DICOMWidget && d->TargetsWidget && d->RegistrationWidget && 
      d->RobotWidget && d->ResliceWidget)
    {
    d->DICOMWidget->setActiveParameterNode(activeNode);
    d->TargetsWidget->setActiveParameterNode(activeNode);
    d->RegistrationWidget->setActiveParameterNode(activeNode);
    d->RobotWidget->setActiveParameterNode(activeNode);
    d->ResliceWidget->setActiveParameterNode(activeNode);
    }
}

//-----------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedModuleWidget::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  return d->getActiveParamNode();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onConnectClicked()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);
  
  if (!this->mrmlScene())
    {
    return;
    }

  vtkMRMLIGTLConnectorNode* currentConnectorNode = d->getConnectorNode();
  vtkMRMLIGTLSessionManagerNode* currentSessionManagerNode = d->getSessionManagerNode();

  // Create Connector node if doesn't exists
  if (!currentConnectorNode)
    {
    vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLIGTLConnectorNode"));
    if (cnode)
      {
      this->mrmlScene()->AddNode(cnode);
      d->setConnectorNode(cnode);
      currentConnectorNode = cnode;
      }
    }

  // Create SessionManager node if doesn't exists
  if (!currentSessionManagerNode)
    {
    vtkMRMLIGTLSessionManagerNode* snode = vtkMRMLIGTLSessionManagerNode::SafeDownCast(
      this->mrmlScene()->CreateNodeByClass("vtkMRMLIGTLSessionManagerNode"));
    if (snode)
      {
      this->mrmlScene()->AddNode(snode);
      d->setSessionManagerNode(snode);
      currentSessionManagerNode = snode;
      }
    }

  if (currentConnectorNode && currentSessionManagerNode)
    {
    switch(currentConnectorNode->GetState())
      {
      case vtkMRMLIGTLConnectorNode::STATE_OFF:
	{
	int port = d->PortWidget->text().toInt();
	std::string hostname(d->HostnameWidget->text().toStdString());
	int type = d->ClientRadioButton->isChecked() ?
	  vtkMRMLIGTLConnectorNode::TYPE_CLIENT :
	  vtkMRMLIGTLConnectorNode::TYPE_SERVER;

	currentConnectorNode->SetType(type);
	currentConnectorNode->SetServerHostname(hostname);
	currentConnectorNode->SetServerPort(port);
	currentConnectorNode->Start();

	currentSessionManagerNode->SetAndObserveConnectorNodeID(currentConnectorNode->GetID());

	this->qvtkConnect(currentConnectorNode, vtkMRMLIGTLConnectorNode::ConnectedEvent,
			  this, SLOT(onConnectorNodeConnected()));
	this->qvtkConnect(currentConnectorNode, vtkMRMLIGTLConnectorNode::DisconnectedEvent,
			  this, SLOT(onConnectorNodeDisconnected()));

	break;
	}

      case vtkMRMLIGTLConnectorNode::STATE_WAIT_CONNECTION:
      case vtkMRMLIGTLConnectorNode::STATE_CONNECTED:
	{
	currentConnectorNode->Stop();

	// BUG: DisconnectedEvent is not triggered when calling Stop() method. Have to call function manually.
	this->onConnectorNodeDisconnected();

	this->qvtkDisconnect(currentConnectorNode, vtkMRMLIGTLConnectorNode::ConnectedEvent,
			  this, SLOT(onConnectorNodeConnected()));
	this->qvtkDisconnect(currentConnectorNode, vtkMRMLIGTLConnectorNode::DisconnectedEvent,
			  this, SLOT(onConnectorNodeDisconnected()));

	this->mrmlScene()->RemoveNode(currentSessionManagerNode);
	this->mrmlScene()->RemoveNode(currentConnectorNode);

	d->setSessionManagerNode(NULL);
	d->setConnectorNode(NULL);

	break;
	}
      }
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onConnectorNodeConnected()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget); 

  d->CommunicationGroupBox->setTitle("Communication : Connected");
  d->ConnectButton->setText("Disconnect");
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onConnectorNodeDisconnected()
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget); 

  d->CommunicationGroupBox->setTitle("Communication : Disconnected");
  d->ConnectButton->setText("Connect");
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onRobotModelChanged(vtkMRMLNode* node)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget); 

  vtkMRMLRobotModelNode* robotNode = 
    vtkMRMLRobotModelNode::SafeDownCast(node);

  vtkSlicerModuleLogic* moduleLogic =
    vtkSlicerModuleLogic::SafeDownCast(this->logic());

  if (moduleLogic && robotNode)
    {    
    robotNode->SetSharedDirectoryPath(moduleLogic->GetModuleShareDirectory().c_str());
    robotNode->SetNeedleLength("175mm");
    d->setRobotModelNode(robotNode);

    vtkMRMLPercutaneousImageGuidedParameterNode* registrationNode =
      d->getRegParamNode();
    if (registrationNode)
      {
      registrationNode->SetAndObserveRobotModelNodeID(robotNode->GetID());
      }
    }
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
      if (d->getRobotModelNode())
	{
	regNode->SetAndObserveRobotModelNodeID(d->getRobotModelNode()->GetID());
	}
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
