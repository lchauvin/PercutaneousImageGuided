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

// Robot Widgets includes
#include "qSlicerPercutaneousImageGuidedRobotWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedRobotWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedRobotWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedRobotWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedRobotWidget);
protected:
  qSlicerPercutaneousImageGuidedRobotWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedRobotWidgetPrivate(
    qSlicerPercutaneousImageGuidedRobotWidget& object);
  ~qSlicerPercutaneousImageGuidedRobotWidgetPrivate();
  virtual void setupUi(qSlicerPercutaneousImageGuidedRobotWidget*);

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRobotWidgetPrivate
::qSlicerPercutaneousImageGuidedRobotWidgetPrivate(
  qSlicerPercutaneousImageGuidedRobotWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRobotWidgetPrivate
::~qSlicerPercutaneousImageGuidedRobotWidgetPrivate()
{
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRobotWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedRobotWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedRobotWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedRobotWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRobotWidget
::qSlicerPercutaneousImageGuidedRobotWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPercutaneousImageGuidedRobotWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedRobotWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedRobotWidget
::~qSlicerPercutaneousImageGuidedRobotWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRobotWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedRobotWidget);

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->activeParamNode;
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::RobotModifiedEvent,
			 this, SLOT(refreshWidget()));
    }

  d->activeParamNode = activeNode;
  this->refreshWidget();

  this->qvtkConnect(d->activeParamNode, vtkMRMLPercutaneousImageGuidedParameterNode::RobotModifiedEvent,
		    this, SLOT(refreshWidget()));
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedRobotWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedRobotWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedRobotWidget::refreshWidget()
{
  std::cerr << "Udpate Robot Widget" << std::endl;
}




