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

#ifndef __qSlicerPercutaneousImageGuidedRobotWidget_h
#define __qSlicerPercutaneousImageGuidedRobotWidget_h

// Qt includes
#include <qSlicerWidget.h>

// Robot Widgets includes
#include "qSlicerPercutaneousImageGuidedModuleWidgetsExport.h"

class qSlicerPercutaneousImageGuidedRobotWidgetPrivate;
class vtkMRMLPercutaneousImageGuidedParameterNode;

/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class Q_SLICER_MODULE_PERCUTANEOUSIMAGEGUIDED_WIDGETS_EXPORT qSlicerPercutaneousImageGuidedRobotWidget
  : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerPercutaneousImageGuidedRobotWidget(QWidget *parent=0);
  virtual ~qSlicerPercutaneousImageGuidedRobotWidget();

  void setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode);
  vtkMRMLPercutaneousImageGuidedParameterNode* getActiveParameterNode(); 

public slots:
  void refreshWidget();

protected:
  QScopedPointer<qSlicerPercutaneousImageGuidedRobotWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerPercutaneousImageGuidedRobotWidget);
  Q_DISABLE_COPY(qSlicerPercutaneousImageGuidedRobotWidget);
};

#endif
