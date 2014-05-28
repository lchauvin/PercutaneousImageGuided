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

#include "ctkDICOMBrowser.h"
#include "ctkDICOMDatabase.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLMarkupsFiducialNode.h"
#include "vtkMRMLSelectionNode.h"
#include "vtkMRMLScene.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPercutaneousImageGuidedModuleWidgetPrivate: public Ui_qSlicerPercutaneousImageGuidedModuleWidget
{
public:
  qSlicerPercutaneousImageGuidedModuleWidgetPrivate();
  ~qSlicerPercutaneousImageGuidedModuleWidgetPrivate();
  int startStoreSCP(QString path, QString aetitle, QString port);
  void stopStoreSCP();
  void showDICOMBrowser();
  void hideDICOMBrowser();

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
  QProcess* storeSCP;
  ctkDICOMBrowser* dicomBrowser;

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
  this->storeSCP = new QProcess();
  this->dicomBrowser = new ctkDICOMBrowser();

  this->PreRegParamNode	 = NULL;
  this->RegParamNode	 = NULL;
  this->PostRegParamNode = NULL;
  this->PostInsParamNode = NULL;
  this->ActiveParamNode  = NULL;
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModuleWidgetPrivate::~qSlicerPercutaneousImageGuidedModuleWidgetPrivate()
{
  if (this->storeSCP)
    {
    this->stopStoreSCP();
    this->storeSCP->deleteLater();
    }

  if (this->dicomBrowser)
    {
    this->dicomBrowser->database()->closeDatabase();
    this->dicomBrowser->deleteLater();
    }

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
int qSlicerPercutaneousImageGuidedModuleWidgetPrivate::startStoreSCP(QString path, QString aetitle, QString port)
{
  if (path.isEmpty() || aetitle.isEmpty() || port.isEmpty())
    {
    return 0;
    }

  // Stop storeSCP if already running
  this->stopStoreSCP();

  // Find storescp (DCMTK)
  std::stringstream storescpBin;
  storescpBin << VTK_DIR << "/../DCMTK-build/bin/storescp";

  // Create parameters
  QStringList storescpParameters;
  storescpParameters << "--output-directory" << path << "-aet" << aetitle << port;

  // Start process
  this->storeSCP->start(storescpBin.str().c_str(), storescpParameters);
  this->storeSCP->waitForStarted(5000);
  
  if (this->storeSCP->state() == QProcess::NotRunning)
    {
    return 0;
    }

  return 1;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidgetPrivate::stopStoreSCP()
{
  if (this->storeSCP->state() == QProcess::NotRunning)
    {
    return;
    }
  
  // Try to terminate process properly
  this->storeSCP->terminate();
  this->storeSCP->waitForFinished(5000);
  if (this->storeSCP->state() == QProcess::Running)
    {
    // if process doesn't stop after 5s, kill it
    this->storeSCP->kill();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidgetPrivate::showDICOMBrowser()
{
  if (this->dicomBrowser)
    {
    this->dicomBrowser->show();
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidgetPrivate::hideDICOMBrowser()
{
  if (this->dicomBrowser)
    {
    this->dicomBrowser->hide();
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
  connect(d->DICOMListenerButton, SIGNAL(directoryChanged(const QString&)),
	  this, SLOT(onDICOMDirectoryChanged(const QString&)));

  connect(d->DICOMManagerButton, SIGNAL(clicked()),
	  this, SLOT(onDICOMManagerClicked()));

  // Targets
  connect(d->TargetListSelector, SIGNAL(currentNodeIDChanged(const QString&)),
	  this, SLOT(onTargetListChanged(const QString&)));

  connect(d->AddTargetButton, SIGNAL(clicked()),
	  this, SLOT(onAddTargetClicked()));

  connect(d->RemoveTargetButton, SIGNAL(clicked()),
	  this, SLOT(onRemoveTargetClicked()));

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
    vtkMRMLPercutaneousImageGuidedParameterNode* preRegNode = vtkMRMLPercutaneousImageGuidedParameterNode::SafeDownCast(
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
void qSlicerPercutaneousImageGuidedModuleWidget::onDICOMDirectoryChanged(const QString& directory)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  QString port = d->DICOMPortBox->text();
  QString aetitle = d->DICOMAETitleBox->text();

  if (d->startStoreSCP(directory, aetitle, port) != 0)
    {
    d->DICOMDirectoryLabel->setText(directory);
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onDICOMManagerClicked()
{   
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);
  
  d->showDICOMBrowser();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onTargetListChanged(const QString& id)
{   
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);
  
  if (id.isEmpty())
    {
    return;
    }

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->getActiveParamNode();
  if (currentActiveNode)
    {
    currentActiveNode->SetAndObserveTargetListNodeID(id.toStdString().c_str());
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onAddTargetClicked()
{   
  if (!this->mrmlScene())
    {
    return;
    }

  vtkMRMLSelectionNode* selectionNode = vtkMRMLSelectionNode::SafeDownCast(
    this->mrmlScene()->GetNodeByID("vtkMRMLSelectionNodeSingleton"));
  vtkMRMLInteractionNode* interactionNode = vtkMRMLInteractionNode::SafeDownCast(
    this->mrmlScene()->GetNodeByID("vtkMRMLInteractionNodeSingleton"));

  if (selectionNode && interactionNode)
    {
    selectionNode->SetReferenceActivePlaceNodeClassName("vtkMRMLMarkupsFiducialNode");
    interactionNode->SwitchToSinglePlaceMode();
    interactionNode->SetCurrentInteractionMode(vtkMRMLInteractionNode::Place);
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::onRemoveTargetClicked()
{   
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!d->TargetListSelector || !d->TargetListWidget)
    {
    return;
    }

  vtkMRMLMarkupsFiducialNode* markupsList = vtkMRMLMarkupsFiducialNode::SafeDownCast(
    this->mrmlScene()->GetNodeByID(d->TargetListSelector->currentNodeID().toStdString().c_str()));
  if (markupsList)
    {
    markupsList->RemoveMarkup(d->TargetListWidget->currentRow());
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModuleWidget::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!activeNode)
    {
    return;
    }
  
  // Disconnect events on previous node
  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->getActiveParamNode();
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::DICOMModifiedEvent,
			 this, SLOT(refreshDICOMWidget()));
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::TargetsModifiedEvent,
			 this, SLOT(refreshTargetsWidget()));
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::WorkspaceModifiedEvent,
			 this, SLOT(refreshWorkspaceWidget()));
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent,
			 this, SLOT(refreshRegistrationWidget()));
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::RobotModifiedEvent,
			 this, SLOT(refreshRobotWidget()));
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::ResliceModifiedEvent,
			 this, SLOT(refreshResliceWidget()));
    }

  // Set new active parameter node
  d->setActiveParamNode(activeNode);

  // Update all widgets (before connecting events to avoid double refresh)
  this->refreshAllWidgets();

  // Connect events on new active node
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::DICOMModifiedEvent,
		       this, SLOT(refreshDICOMWidget()));
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::TargetsModifiedEvent,
		       this, SLOT(refreshTargetsWidget()));
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::WorkspaceModifiedEvent,
		       this, SLOT(refreshWorkspaceWidget()));
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent,
		       this, SLOT(refreshRegistrationWidget()));
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::RobotModifiedEvent,
		       this, SLOT(refreshRobotWidget()));
  this->qvtkConnect(activeNode, vtkMRMLPercutaneousImageGuidedParameterNode::ResliceModifiedEvent,
		       this, SLOT(refreshResliceWidget()));
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
  Q_D(qSlicerPercutaneousImageGuidedModuleWidget);

  if (!this->mrmlScene() || 
      !d->TargetListSelector || !d->TargetListWidget)
    {
    return;
    }

  std::cerr << "Update Targets Widget" << std::endl;

  // Clear table widget
  d->TargetListWidget->clearContents();
  d->TargetListWidget->setRowCount(0);

  // Set new active target list
  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->getActiveParamNode();
  if (currentActiveNode)
    {
    const char* targetNodeID = currentActiveNode->GetTargetListNodeID();
    d->TargetListSelector->setCurrentNodeID(targetNodeID);

    // Re-populate table widget 
    vtkMRMLMarkupsFiducialNode* markupsList = vtkMRMLMarkupsFiducialNode::SafeDownCast(
      this->mrmlScene()->GetNodeByID(targetNodeID));
    if (markupsList)
      {
      int numberOfFiducials = markupsList->GetNumberOfFiducials();
      for (int i = 0; i < numberOfFiducials; ++i)
	{
	d->TargetListWidget->insertRow(i);

	double targetPosition[3] = {0.0, 0.0, 0.0};
	markupsList->GetNthFiducialPosition(i,targetPosition);
	Qt::CheckState targetVisibility = markupsList->GetNthFiducialVisibility(i) ? 
	  Qt::Checked : 
	  Qt::Unchecked;

	QTableWidgetItem*	targetName = new QTableWidgetItem(
	  QString::fromStdString(markupsList->GetNthFiducialLabel(i)));
	QTableWidgetItem*	targetR	   = new QTableWidgetItem(QString::number(targetPosition[0]));
	QTableWidgetItem*	targetA	   = new QTableWidgetItem(QString::number(targetPosition[1]));
	QTableWidgetItem*	targetS	   = new QTableWidgetItem(QString::number(targetPosition[2]));
	QTableWidgetItem*	targetVis  = new QTableWidgetItem();
	targetVis->setCheckState(targetVisibility);

	d->TargetListWidget->setItem(i,0,targetName);
	d->TargetListWidget->setItem(i,1,targetR);
	d->TargetListWidget->setItem(i,2,targetA);
	d->TargetListWidget->setItem(i,3,targetS);
	d->TargetListWidget->setItem(i,4,targetVis);
	}
      }
    }  
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
