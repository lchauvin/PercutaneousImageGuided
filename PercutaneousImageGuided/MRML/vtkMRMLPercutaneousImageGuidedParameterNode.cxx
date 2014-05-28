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
 
  This file was originally developed by Laurent Chauvin, Brigham and Women's
  Hospital. The project was supported by grants 5P01CA067165,
  5R01CA124377, 5R01CA138586, 2R44DE019322, 7R01CA124377,
  5R42CA137886, 8P41EB015898
 
==============================================================================*/

#include "vtkMRMLPercutaneousImageGuidedParameterNode.h"

#include <vtkObjectFactory.h>

const char* vtkMRMLPercutaneousImageGuidedParameterNode::RobotModelNodeReferenceRole = "robotModel";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::RobotModelNodeReferenceMRMLAttributeName = "robotModelNodeRef";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::TargetListNodeReferenceRole = "targetList";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::TargetListNodeReferenceMRMLAttributeName = "targetListNodeRef";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::FiducialListNodeReferenceRole = "fiducialList";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::FiducialListNodeReferenceMRMLAttributeName = "fiducialListNodeRef";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationNodeReferenceRole = "registrationTransform";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationNodeReferenceMRMLAttributeName = "registrationTransformNodeRef";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::TrajectoryListNodeReferenceRole = "trajectoryList";
const char* vtkMRMLPercutaneousImageGuidedParameterNode::TrajectoryListNodeReferenceMRMLAttributeName = "trajectoryListNodeRef";

//----------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLPercutaneousImageGuidedParameterNode);

//----------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode::vtkMRMLPercutaneousImageGuidedParameterNode()
{
  this->AddNodeReferenceRole(this->GetRobotModelNodeReferenceRole(),
			     this->GetRobotModelNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetTargetListNodeReferenceRole(),
			     this->GetTargetListNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetFiducialListNodeReferenceRole(),
			     this->GetFiducialListNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetRegistrationNodeReferenceRole(),
			     this->GetRegistrationNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetTrajectoryListNodeReferenceRole(),
			     this->GetTrajectoryListNodeReferenceMRMLAttributeName());
}

//----------------------------------------------------------------------------
vtkMRMLPercutaneousImageGuidedParameterNode::~vtkMRMLPercutaneousImageGuidedParameterNode()
{
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRobotModelNodeReferenceRole()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::RobotModelNodeReferenceRole;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRobotModelNodeReferenceMRMLAttributeName()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::RobotModelNodeReferenceMRMLAttributeName;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTargetListNodeReferenceRole()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::TargetListNodeReferenceRole;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTargetListNodeReferenceMRMLAttributeName()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::TargetListNodeReferenceMRMLAttributeName;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetFiducialListNodeReferenceRole()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::FiducialListNodeReferenceRole;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetFiducialListNodeReferenceMRMLAttributeName()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::FiducialListNodeReferenceMRMLAttributeName;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRegistrationNodeReferenceRole()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationNodeReferenceRole;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRegistrationNodeReferenceMRMLAttributeName()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationNodeReferenceMRMLAttributeName;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTrajectoryListNodeReferenceRole()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::TrajectoryListNodeReferenceRole;
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTrajectoryListNodeReferenceMRMLAttributeName()
{
  return vtkMRMLPercutaneousImageGuidedParameterNode::TrajectoryListNodeReferenceMRMLAttributeName;
}

//----------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetScene(vtkMRMLScene* scene)
{
  this->Superclass::SetScene(scene);
}

//----------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetNodeTagName()
{
  return "StepParameter";
}

//----------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
}

//----------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
}


//----------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::ReadXMLAttributes(const char** atts)
{
  Superclass::ReadXMLAttributes(atts);
}

//----------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::Copy(vtkMRMLNode *anode)
{
  Superclass::Copy(anode);
}

//-----------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::UpdateScene(vtkMRMLScene *scene)
{
  Superclass::UpdateScene(scene);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::ProcessMRMLEvents ( vtkObject *caller,
                                           unsigned long event, 
                                           void *callData )
{
  Superclass::ProcessMRMLEvents(caller, event, callData);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::ApplyTransformMatrix(vtkMatrix4x4* transformMatrix)
{
  Superclass::ApplyTransformMatrix(transformMatrix);
}
							

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetAndObserveRobotModelNodeID (const char* robotModelNodeID)
{
  this->SetAndObserveNodeReferenceID(this->GetRobotModelNodeReferenceRole(), robotModelNodeID);
  this->InvokeEvent(vtkMRMLPercutaneousImageGuidedParameterNode::RobotModifiedEvent);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetAndObserveTargetListNodeID (const char* targetListNodeID)
{
  if (!this->GetScene())
    {
    return;
    }

  // ISSUE: If multiple parameters use the same target list, it's not possible to have
  // same markups list under 2 different parameter nodes

  // Remove markups list if previous one
  /*
  vtkMRMLDisplayableNode* previousTargetList = vtkMRMLDisplayableNode::SafeDownCast(
    this->GetScene()->GetNodeByID(this->GetTargetListNodeID()));
  if (previousTargetList)
    {
    previousTargetList->SetAndObserveTransformNodeID(NULL);
    }
  */

  // Update Target List Node ID
  this->SetAndObserveNodeReferenceID(this->GetTargetListNodeReferenceRole(), targetListNodeID);

  // Set new markup list under this node
  /*
  vtkMRMLDisplayableNode* newTargetList = vtkMRMLDisplayableNode::SafeDownCast(
    this->GetScene()->GetNodeByID(targetListNodeID));
  if (newTargetList)
    {
    newTargetList->SetAndObserveTransformNodeID(this->GetID());
    }
  */
  this->InvokeEvent(vtkMRMLPercutaneousImageGuidedParameterNode::TargetsModifiedEvent);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetAndObserveFiducialListNodeID (const char* fiducialListNodeID)
{
  this->SetAndObserveNodeReferenceID(this->GetFiducialListNodeReferenceRole(), fiducialListNodeID);
  this->InvokeEvent(vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetAndObserveRegistrationNodeID (const char* registrationNodeID)
{
  this->SetAndObserveNodeReferenceID(this->GetRegistrationNodeReferenceRole(), registrationNodeID);
  this->InvokeEvent(vtkMRMLPercutaneousImageGuidedParameterNode::RegistrationModifiedEvent);
}

//---------------------------------------------------------------------------
void vtkMRMLPercutaneousImageGuidedParameterNode::SetAndObserveTrajectoryListNodeID (const char* trajectoryListNodeID)
{
  this->SetAndObserveNodeReferenceID(this->GetTrajectoryListNodeReferenceRole(), trajectoryListNodeID);
  this->InvokeEvent(vtkMRMLPercutaneousImageGuidedParameterNode::ResliceModifiedEvent);
}

//---------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRobotModelNodeID ()
{
  return this->GetNodeReferenceID(this->GetRobotModelNodeReferenceRole());
}

//---------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTargetListNodeID ()
{
  return this->GetNodeReferenceID(this->GetTargetListNodeReferenceRole());
}

//---------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetFiducialListNodeID ()
{
  return this->GetNodeReferenceID(this->GetFiducialListNodeReferenceRole());
}

//---------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetRegistrationNodeID ()
{
  return this->GetNodeReferenceID(this->GetRegistrationNodeReferenceRole());
}

//---------------------------------------------------------------------------
const char* vtkMRMLPercutaneousImageGuidedParameterNode::GetTrajectoryListNodeID ()
{
  return this->GetNodeReferenceID(this->GetTrajectoryListNodeReferenceRole());
}

