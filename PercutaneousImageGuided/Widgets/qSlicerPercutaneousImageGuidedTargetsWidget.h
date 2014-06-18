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

#ifndef __qSlicerPercutaneousImageGuidedTargetsWidget_h
#define __qSlicerPercutaneousImageGuidedTargetsWidget_h

// Qt includes
#include <qSlicerWidget.h>
#include <QTableWidgetItem>

// Targets Widgets includes
#include "qSlicerPercutaneousImageGuidedModuleWidgetsExport.h"

class qSlicerPercutaneousImageGuidedTargetsWidgetPrivate;
class vtkMRMLPercutaneousImageGuidedParameterNode;

/// \ingroup Slicer_QtModules_PercutaneousImageGuided
class Q_SLICER_MODULE_PERCUTANEOUSIMAGEGUIDED_WIDGETS_EXPORT qSlicerPercutaneousImageGuidedTargetsWidget
  : public qSlicerWidget
{
  Q_OBJECT
public:
  typedef qSlicerWidget Superclass;
  qSlicerPercutaneousImageGuidedTargetsWidget(QWidget *parent=0);
  virtual ~qSlicerPercutaneousImageGuidedTargetsWidget();

  // Columns of target list widget
  enum
  {
    TARGET_VISIBILITY = 0,
    TARGET_NAME,
    TARGET_R,
    TARGET_A,
    TARGET_S
  };

  void setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode);
  vtkMRMLPercutaneousImageGuidedParameterNode* getActiveParameterNode(); 
  
public slots:
  void refreshWidget();

  void onTargetListChanged(const QString& id);
  void onAddTargetClicked();
  void onRemoveTargetClicked();
  void onTargetItemChanged(QTableWidgetItem* itemModified);

protected:
  QScopedPointer<qSlicerPercutaneousImageGuidedTargetsWidgetPrivate> d_ptr;

private:
  Q_DECLARE_PRIVATE(qSlicerPercutaneousImageGuidedTargetsWidget);
  Q_DISABLE_COPY(qSlicerPercutaneousImageGuidedTargetsWidget);
};

#endif
