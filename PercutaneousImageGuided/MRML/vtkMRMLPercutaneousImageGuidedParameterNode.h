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

#ifndef __vtkMRMLPercutaneousImageGuidedParameterNode_h
#define __vtkMRMLPercutaneousImageGuidedParameterNode_h

#include <iostream>

#include "vtkSlicerPercutaneousImageGuidedModuleMRMLExport.h"

#include "vtkMRMLDisplayableNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLScene.h"

class  VTK_SLICER_PERCUTANEOUSIMAGEGUIDED_MODULE_MRML_EXPORT vtkMRMLPercutaneousImageGuidedParameterNode : 
public vtkMRMLLinearTransformNode
{
 public:
  static vtkMRMLPercutaneousImageGuidedParameterNode *New();
  vtkTypeMacro(vtkMRMLPercutaneousImageGuidedParameterNode,vtkMRMLLinearTransformNode);
  void PrintSelf(ostream& os, vtkIndent indent);

  enum 
  {
    DICOMModifiedEvent = 42000,
    TargetsModifiedEvent = 42001,
    WorkspaceModifiedEvent = 42002,
    RegistrationModifiedEvent = 42003,
    RobotModifiedEvent = 42004,
    ResliceModifiedEvent = 42005,
  };

  //--------------------------------------------------------------------------
  // MRMLNode methods
  //--------------------------------------------------------------------------

  virtual void SetScene(vtkMRMLScene* scene);

  virtual vtkMRMLNode* CreateNodeInstance();

  /// 
  /// Read node attributes from XML file
  virtual void ReadXMLAttributes( const char** atts);

  // Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  virtual void UpdateScene(vtkMRMLScene *scene);

  // Description:
  // Copy the node's attributes to this object
  virtual void Copy(vtkMRMLNode *node);

  // Description:
  // alternative method to propagate events generated in Display nodes
  virtual void ProcessMRMLEvents ( vtkObject * /*caller*/,
                                   unsigned long /*event*/,
                                   void * /*callData*/ );

  virtual void ApplyTransformMatrix(vtkMatrix4x4* transformMatrix);

  // Description:
  // Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName();

  void SetAndObserveRobotModelNodeID(const char* robotModelNodeID);
  void SetAndObserveTargetListNodeID(const char* targetListNodeID);
  void SetAndObserveFiducialListNodeID(const char* fiducialListNodeID);
  void SetAndObserveRegistrationNodeID(const char* registrationNodeID);
  void SetAndObserveTrajectoryListNodeID(const char* trajectoryListNodeID);

  const char* GetRobotModelNodeID();
  const char* GetTargetListNodeID();
  const char* GetFiducialListNodeID();
  const char* GetRegistrationNodeID();
  const char* GetTrajectoryListNodeID();

 protected:
  vtkMRMLPercutaneousImageGuidedParameterNode();
  ~vtkMRMLPercutaneousImageGuidedParameterNode();
  vtkMRMLPercutaneousImageGuidedParameterNode(const vtkMRMLPercutaneousImageGuidedParameterNode&);
  void operator=(const vtkMRMLPercutaneousImageGuidedParameterNode&);

  // Robot Model Node Reference
  static const char* RobotModelNodeReferenceRole;
  static const char* RobotModelNodeReferenceMRMLAttributeName;

  virtual const char* GetRobotModelNodeReferenceRole();
  virtual const char* GetRobotModelNodeReferenceMRMLAttributeName();

  // Target List Node Reference
  static const char* TargetListNodeReferenceRole;
  static const char* TargetListNodeReferenceMRMLAttributeName;

  virtual const char* GetTargetListNodeReferenceRole();
  virtual const char* GetTargetListNodeReferenceMRMLAttributeName();

  // Fiducial List Node Reference
  static const char* FiducialListNodeReferenceRole;
  static const char* FiducialListNodeReferenceMRMLAttributeName;

  virtual const char* GetFiducialListNodeReferenceRole();
  virtual const char* GetFiducialListNodeReferenceMRMLAttributeName();

  // Registration Node Reference
  static const char* RegistrationNodeReferenceRole;
  static const char* RegistrationNodeReferenceMRMLAttributeName;

  virtual const char* GetRegistrationNodeReferenceRole();
  virtual const char* GetRegistrationNodeReferenceMRMLAttributeName();

  // Trajectory List Node Reference
  static const char* TrajectoryListNodeReferenceRole;
  static const char* TrajectoryListNodeReferenceMRMLAttributeName;

  virtual const char* GetTrajectoryListNodeReferenceRole();
  virtual const char* GetTrajectoryListNodeReferenceMRMLAttributeName();

  ///
  /// Called when a node reference ID is added (list size increased).
  virtual void OnNodeReferenceAdded(vtkMRMLNodeReference *reference)
  {
    Superclass::OnNodeReferenceAdded(reference);
  }

  ///
  /// Called when a node reference ID is modified.
  virtual void OnNodeReferenceModified(vtkMRMLNodeReference *reference)
  {
    Superclass::OnNodeReferenceModified(reference);
  }

  ///
  /// Called after a node reference ID is removed (list size decreased).
  virtual void OnNodeReferenceRemoved(vtkMRMLNodeReference *reference)
  {
    Superclass::OnNodeReferenceRemoved(reference);
  }
};

#endif
