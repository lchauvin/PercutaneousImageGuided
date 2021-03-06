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

// Targets Widgets includes
#include "qSlicerPercutaneousImageGuidedTargetsWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedTargetsWidget.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"
#include "vtkMRMLInteractionNode.h"
#include "vtkMRMLMarkupsFiducialNode.h"
#include "vtkMRMLSelectionNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedTargetsWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedTargetsWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedTargetsWidget);
protected:
  qSlicerPercutaneousImageGuidedTargetsWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedTargetsWidgetPrivate(
    qSlicerPercutaneousImageGuidedTargetsWidget& object);
  virtual void setupUi(qSlicerPercutaneousImageGuidedTargetsWidget*);

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;
  vtkMRMLMarkupsFiducialNode* previousTargetList;
  vtkMRMLMarkupsFiducialNode* activeTargetList;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedTargetsWidgetPrivate
::qSlicerPercutaneousImageGuidedTargetsWidgetPrivate(
  qSlicerPercutaneousImageGuidedTargetsWidget& object)
  : q_ptr(&object)
{
  this->activeParamNode = NULL;
  this->previousTargetList = NULL;
  this->activeTargetList = NULL;
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedTargetsWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedTargetsWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedTargetsWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedTargetsWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedTargetsWidget
::qSlicerPercutaneousImageGuidedTargetsWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
    , d_ptr( new qSlicerPercutaneousImageGuidedTargetsWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);
  d->setupUi(this);

  connect(d->TargetListSelector, SIGNAL(currentNodeIDChanged(const QString&)),
          this, SLOT(onTargetListChanged(const QString&)));

  connect(d->AddTargetButton, SIGNAL(clicked()),
          this, SLOT(onAddTargetClicked()));

  connect(d->RemoveTargetButton, SIGNAL(clicked()),
          this, SLOT(onRemoveTargetClicked()));

  connect(d->TargetListWidget, SIGNAL(itemChanged(QTableWidgetItem*)),
          this, SLOT(onTargetItemChanged(QTableWidgetItem*)));
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedTargetsWidget
::~qSlicerPercutaneousImageGuidedTargetsWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedTargetsWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);

  if (d->activeParamNode != activeNode)
    {
    d->activeParamNode = activeNode;
    }

  if (d->activeParamNode)
    {
    d->TargetListSelector->setCurrentNodeID(d->activeParamNode->GetTargetListNodeID());
    }
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedTargetsWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedTargetsWidget::onTargetListChanged(const QString& id)
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);

  if (id.isEmpty())
    {
    d->TargetListWidget->clearContents();
    d->TargetListWidget->setRowCount(0);
    return;
    }

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = this->getActiveParameterNode();
  if (currentActiveNode)
    {
    const char* targetListNodeID = id.toStdString().c_str();
    currentActiveNode->SetAndObserveTargetListNodeID(targetListNodeID);
    d->activeTargetList = vtkMRMLMarkupsFiducialNode::SafeDownCast(
      this->mrmlScene()->GetNodeByID(targetListNodeID));
    }

  // Observe Markups events
  if (d->activeTargetList)
    {
    // Update widget with new target list
    this->refreshWidget();

    // Observe Markups events
    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::LockModifiedEvent,
                        this, SLOT(refreshWidget()));

    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::LabelFormatModifiedEvent,
                        this, SLOT(refreshWidget()));

    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::PointModifiedEvent,
                        this, SLOT(refreshWidget()));

    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::NthMarkupModifiedEvent,
                        this, SLOT(refreshWidget()));

    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::MarkupRemovedEvent,
                        this, SLOT(refreshWidget()));

    this->qvtkReconnect(d->previousTargetList,
                        d->activeTargetList, vtkMRMLMarkupsFiducialNode::MarkupAddedEvent,
                        this, SLOT(refreshWidget()));

    // Save current target list
    d->previousTargetList = d->activeTargetList;
    }
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedTargetsWidget::onAddTargetClicked()
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
void qSlicerPercutaneousImageGuidedTargetsWidget::onRemoveTargetClicked()
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);

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
void qSlicerPercutaneousImageGuidedTargetsWidget::refreshWidget()
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);

  if (!this->mrmlScene() ||
      !d->TargetListSelector || !d->TargetListWidget)
    {
    return;
    }

  std::cerr << "Update Targets Widget" << std::endl;

  bool blockSignal = d->TargetListWidget->blockSignals(true);

  // Clear table widget
  d->TargetListWidget->clearContents();
  d->TargetListWidget->setRowCount(0);

  if (d->activeTargetList)
    {
    int numberOfFiducials = d->activeTargetList->GetNumberOfFiducials();
    for (int i = 0; i < numberOfFiducials; ++i)
      {
      d->TargetListWidget->insertRow(i);

      // Get Markup data
      QString targetName = QString::fromStdString(d->activeTargetList->GetNthFiducialLabel(i));
      double targetPosition[3] = {0.0, 0.0, 0.0};
      d->activeTargetList->GetNthFiducialPosition(i,targetPosition);

      QString targetPositionR = QString::number(targetPosition[0]);
      QString targetPositionA = QString::number(targetPosition[1]);
      QString targetPositionS = QString::number(targetPosition[2]);

      // Create items
      QTableWidgetItem* itemTargetName = new QTableWidgetItem(targetName);
      QTableWidgetItem* itemTargetR    = new QTableWidgetItem(targetPositionR);
      QTableWidgetItem* itemTargetA    = new QTableWidgetItem(targetPositionA);
      QTableWidgetItem* itemTargetS    = new QTableWidgetItem(targetPositionS);
      QTableWidgetItem* itemTargetVis  = new QTableWidgetItem();

      // Update visibility
      itemTargetVis->setCheckState(d->activeTargetList->GetNthFiducialVisibility(i) ?
                                   Qt::Checked :
                                   Qt::Unchecked);

      // Update table widget
      d->TargetListWidget->setItem(i,TARGET_VISIBILITY,itemTargetVis);
      d->TargetListWidget->setItem(i,TARGET_NAME,itemTargetName);
      d->TargetListWidget->setItem(i,TARGET_R,itemTargetR);
      d->TargetListWidget->setItem(i,TARGET_A,itemTargetA);
      d->TargetListWidget->setItem(i,TARGET_S,itemTargetS);
      }
    }

  d->TargetListWidget->blockSignals(blockSignal);
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedTargetsWidget::onTargetItemChanged(QTableWidgetItem* itemModified)
{
  Q_D(qSlicerPercutaneousImageGuidedTargetsWidget);
  
  if (!d->activeTargetList || !itemModified || !d->TargetListWidget)
    {
    return;
    }

  int itemModifiedRow = itemModified->row();
  switch(itemModified->column())
    {
    case TARGET_VISIBILITY:
      {
      bool targetVisibility = itemModified->checkState() == Qt::Checked;
      d->activeTargetList->SetNthFiducialVisibility(itemModifiedRow,targetVisibility);
      break;
      }
    case TARGET_NAME:
      {
      QString targetName = itemModified->text();
      d->activeTargetList->SetNthFiducialLabel(itemModifiedRow,targetName.toStdString());
      break;
      }
    case TARGET_R:
    case TARGET_A:
    case TARGET_S:
      {
      double r = d->TargetListWidget->item(itemModifiedRow,TARGET_R)->text().toDouble();
      double a = d->TargetListWidget->item(itemModifiedRow,TARGET_A)->text().toDouble();
      double s = d->TargetListWidget->item(itemModifiedRow,TARGET_S)->text().toDouble();
      d->activeTargetList->SetNthFiducialPosition(itemModifiedRow,r,a,s);
      break;
      }
    }
}
