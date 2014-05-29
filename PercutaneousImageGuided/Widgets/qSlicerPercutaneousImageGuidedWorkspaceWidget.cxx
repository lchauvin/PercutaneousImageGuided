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

// Workspace Widgets includes
#include "qSlicerPercutaneousImageGuidedWorkspaceWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedWorkspaceWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedWorkspaceWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedWorkspaceWidget);
protected:
  qSlicerPercutaneousImageGuidedWorkspaceWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate(
    qSlicerPercutaneousImageGuidedWorkspaceWidget& object);
  ~qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate();
  virtual void setupUi(qSlicerPercutaneousImageGuidedWorkspaceWidget*);

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate
::qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate(
  qSlicerPercutaneousImageGuidedWorkspaceWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate
::~qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate()
{
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedWorkspaceWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedWorkspaceWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedWorkspaceWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedWorkspaceWidget
::qSlicerPercutaneousImageGuidedWorkspaceWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedWorkspaceWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedWorkspaceWidget
::~qSlicerPercutaneousImageGuidedWorkspaceWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedWorkspaceWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedWorkspaceWidget);

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->activeParamNode;
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::WorkspaceModifiedEvent,
			 this, SLOT(refreshWidget()));
    }

  d->activeParamNode = activeNode;
  this->refreshWidget();

  this->qvtkConnect(d->activeParamNode, vtkMRMLPercutaneousImageGuidedParameterNode::WorkspaceModifiedEvent,
		    this, SLOT(refreshWidget()));
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedWorkspaceWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedWorkspaceWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedWorkspaceWidget::refreshWidget()
{
  std::cerr << "Udpate Workspace Widget" << std::endl;
}




