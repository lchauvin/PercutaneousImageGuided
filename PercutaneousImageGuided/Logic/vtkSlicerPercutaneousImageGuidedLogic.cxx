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

// PercutaneousImageGuided Logic includes
#include "vtkSlicerPercutaneousImageGuidedLogic.h"

// MRML includes
#include "vtkMRMLIGTLSessionManagerNode.h"
#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"
#include <vtkMRMLScene.h>

// VTK includes
#include <vtkIntArray.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

// STD includes
#include <cassert>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerPercutaneousImageGuidedLogic);

//----------------------------------------------------------------------------
vtkSlicerPercutaneousImageGuidedLogic::vtkSlicerPercutaneousImageGuidedLogic()
{
}

//----------------------------------------------------------------------------
vtkSlicerPercutaneousImageGuidedLogic::~vtkSlicerPercutaneousImageGuidedLogic()
{
}

//----------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//---------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}

//-----------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic::RegisterNodes()
{
  assert(this->GetMRMLScene() != 0);

  vtkSmartPointer<vtkMRMLPercutaneousImageGuidedParameterNode> percutaneousImageGuidedParameterNode=
    vtkSmartPointer<vtkMRMLPercutaneousImageGuidedParameterNode>::New();
  this->GetMRMLScene()->RegisterNodeClass(percutaneousImageGuidedParameterNode.GetPointer());

  vtkSmartPointer<vtkMRMLIGTLSessionManagerNode> sessionManagerNode =
    vtkSmartPointer<vtkMRMLIGTLSessionManagerNode>::New();
  this->GetMRMLScene()->RegisterNodeClass(sessionManagerNode.GetPointer());
}

//---------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic::UpdateFromMRMLScene()
{
  assert(this->GetMRMLScene() != 0);
}

//---------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic
::OnMRMLSceneNodeAdded(vtkMRMLNode* vtkNotUsed(node))
{
}

//---------------------------------------------------------------------------
void vtkSlicerPercutaneousImageGuidedLogic
::OnMRMLSceneNodeRemoved(vtkMRMLNode* vtkNotUsed(node))
{
}

