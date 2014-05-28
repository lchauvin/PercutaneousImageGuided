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

// Qt includes
#include <QtPlugin>

// PercutaneousImageGuided Logic includes
#include <vtkSlicerPercutaneousImageGuidedLogic.h>

// PercutaneousImageGuided includes
#include "qSlicerPercutaneousImageGuidedModule.h"
#include "qSlicerPercutaneousImageGuidedModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerPercutaneousImageGuidedModule, qSlicerPercutaneousImageGuidedModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerPercutaneousImageGuidedModulePrivate
{
public:
  qSlicerPercutaneousImageGuidedModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModulePrivate
::qSlicerPercutaneousImageGuidedModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerPercutaneousImageGuidedModule methods

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModule
::qSlicerPercutaneousImageGuidedModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerPercutaneousImageGuidedModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerPercutaneousImageGuidedModule::~qSlicerPercutaneousImageGuidedModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerPercutaneousImageGuidedModule::helpText()const
{
  return "This is a loadable module bundled in an extension";
}

//-----------------------------------------------------------------------------
QString qSlicerPercutaneousImageGuidedModule::acknowledgementText()const
{
  return "This work was was partially funded by NIH grant 3P41RR013218-12S1";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPercutaneousImageGuidedModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString("Laurent Chauvin (SNR)");
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerPercutaneousImageGuidedModule::icon()const
{
  return QIcon(":/Icons/PercutaneousImageGuided.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerPercutaneousImageGuidedModule::categories() const
{
  return QStringList() << "IGT";
}

//-----------------------------------------------------------------------------
QStringList qSlicerPercutaneousImageGuidedModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerPercutaneousImageGuidedModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerPercutaneousImageGuidedModule
::createWidgetRepresentation()
{
  return new qSlicerPercutaneousImageGuidedModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerPercutaneousImageGuidedModule::createLogic()
{
  return vtkSlicerPercutaneousImageGuidedLogic::New();
}
