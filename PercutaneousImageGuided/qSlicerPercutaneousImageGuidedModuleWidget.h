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

#ifndef __qSlicerPercutaneousImageGuidedModuleWidget_h
#define __qSlicerPercutaneousImageGuidedModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"

#include "qSlicerPercutaneousImageGuidedModuleExport.h"

class qSlicerPercutaneousImageGuidedModuleWidgetPrivate;
class vtkMRMLPercutaneousImageGuidedParameterNode;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_PERCUTANEOUSIMAGEGUIDED_EXPORT qSlicerPercutaneousImageGuidedModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerPercutaneousImageGuidedModuleWidget(QWidget *parent=0);
  virtual ~qSlicerPercutaneousImageGuidedModuleWidget();

  // Tabs order
  enum
  {
    DICOM_TAB = 0,
    TARGETS_TAB,
    WORKSPACE_TAB,
    REGISTRATION_TAB,
    ROBOT_TAB,
    RESLICE_TAB
  };
  
  virtual void enter();
  virtual void exit();

public slots:
  void onPreRegButtonToggled(bool pressed);
  void onRegButtonToggled(bool pressed);
  void onPostRegButtonToggled(bool pressed);
  void onPostInsButtonToggled(bool pressed);

  // DICOM

  // Targets

  // Parameter Nodes
  void setActiveParameterNode(vtkMRMLPercutaneousImageGuidedParameterNode* activeNode);

  // Refresh widgets
  void refreshAllWidgets();
  void refreshDICOMWidget();
  void refreshTargetsWidget();
  void refreshWorkspaceWidget();
  void refreshRegistrationWidget();
  void refreshRobotWidget();
  void refreshResliceWidget();

protected:
  QScopedPointer<qSlicerPercutaneousImageGuidedModuleWidgetPrivate> d_ptr;
  
  virtual void setup();

private:
  Q_DECLARE_PRIVATE(qSlicerPercutaneousImageGuidedModuleWidget);
  Q_DISABLE_COPY(qSlicerPercutaneousImageGuidedModuleWidget);
};

#endif
