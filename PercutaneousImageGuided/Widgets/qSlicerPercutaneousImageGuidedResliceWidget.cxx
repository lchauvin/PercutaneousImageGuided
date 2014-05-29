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

// Reslice Widgets includes
#include "qSlicerPercutaneousImageGuidedResliceWidget.h"
#include "ui_qSlicerPercutaneousImageGuidedResliceWidget.h"

#include "vtkSlicerConfigure.h"

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class qSlicerPercutaneousImageGuidedResliceWidgetPrivate
  : public Ui_qSlicerPercutaneousImageGuidedResliceWidget
{
  Q_DECLARE_PUBLIC(qSlicerPercutaneousImageGuidedResliceWidget);
protected:
  qSlicerPercutaneousImageGuidedResliceWidget* const q_ptr;

public:
  qSlicerPercutaneousImageGuidedResliceWidgetPrivate(
    qSlicerPercutaneousImageGuidedResliceWidget& object);
  ~qSlicerPercutaneousImageGuidedResliceWidgetPrivate();
  virtual void setupUi(qSlicerPercutaneousImageGuidedResliceWidget*);

private:
  vtkMRMLPercutaneousImageGuidedParameterNode* activeParamNode;
};

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedResliceWidgetPrivate
::qSlicerPercutaneousImageGuidedResliceWidgetPrivate(
  qSlicerPercutaneousImageGuidedResliceWidget& object)
  : q_ptr(&object)
{
}

// --------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedResliceWidgetPrivate
::~qSlicerPercutaneousImageGuidedResliceWidgetPrivate()
{
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedResliceWidgetPrivate
::setupUi(qSlicerPercutaneousImageGuidedResliceWidget* widget)
{
  this->Ui_qSlicerPercutaneousImageGuidedResliceWidget::setupUi(widget);
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedResliceWidget methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedResliceWidget
::qSlicerPercutaneousImageGuidedResliceWidget(QWidget* parentWidget)
  : Superclass( parentWidget )
  , d_ptr( new qSlicerPercutaneousImageGuidedResliceWidgetPrivate(*this) )
{
  Q_D(qSlicerPercutaneousImageGuidedResliceWidget);
  d->setupUi(this);
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedResliceWidget
::~qSlicerPercutaneousImageGuidedResliceWidget()
{
  this->qvtkDisconnectAll();
}

// --------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedResliceWidget
::setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode)
{
  Q_D(qSlicerPercutaneousImageGuidedResliceWidget);

  vtkMRMLPercutaneousImageGuidedParameterNode* currentActiveNode = d->activeParamNode;
  if (currentActiveNode)
    {
    this->qvtkDisconnect(currentActiveNode, vtkMRMLPercutaneousImageGuidedParameterNode::ResliceModifiedEvent,
			 this, SLOT(refreshWidget()));
    }

  d->activeParamNode = activeNode;
  this->refreshWidget();

  this->qvtkConnect(d->activeParamNode, vtkMRMLPercutaneousImageGuidedParameterNode::ResliceModifiedEvent,
		    this, SLOT(refreshWidget()));
}

// --------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode* qSlicerPercutaneousImageGuidedResliceWidget
::getActiveParameterNode()
{
  Q_D(qSlicerPercutaneousImageGuidedResliceWidget);

  return d->activeParamNode;
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedResliceWidget::refreshWidget()
{
  std::cerr << "Udpate Reslice Widget" << std::endl;
}




