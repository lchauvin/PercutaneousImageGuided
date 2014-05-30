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

#ifndef __qSlicerPercutaneousImageGuidedWorkspaceWidget_h
#define __qSlicerPercutaneousImageGuidedWorkspaceWidget_h

// Qt includes
#include <qSlicerWidget.h>

// Workspace Widgets includes
#include "qSlicerPercutaneousImageGuidedModuleWidgetsExport.h"

class qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate;
class vtkMRMLPercutaneousImageGuidedParameterNode;

/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class Q_SLICER_MODULE_PERCUTANEOUSIMAGEGUIDED_WIDGETS_EXPORT qSlicerPercutaneousImageGuidedWorkspaceWidget
  : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerPercutaneousImageGuidedWorkspaceWidget(QWidget *parent=0);
  virtual ~qSlicerPercutaneousImageGuidedWorkspaceWidget();

  void setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode);
  vtkMRMLPercutaneousImageGuidedParameterNode* getActiveParameterNode(); 

public slots:
  void refreshWidget();

protected:
  QScopedPointer<qSlicerPercutaneousImageGuidedWorkspaceWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerPercutaneousImageGuidedWorkspaceWidget);
  Q_DISABLE_COPY(qSlicerPercutaneousImageGuidedWorkspaceWidget);
};

#endif
