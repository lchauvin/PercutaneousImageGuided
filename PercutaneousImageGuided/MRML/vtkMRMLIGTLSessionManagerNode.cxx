/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer

  =========================================================================auto=*/

// VTK includes
#include <vtkCommand.h>
#include <vtkIntArray.h>
#include <vtkMatrixToLinearTransform.h>
#include <vtkObjectFactory.h>

// MRML includes
#include "vtkMRMLScene.h"
#include "vtkMRMLIGTLSessionManagerNode.h"
#include "vtkMRMLIGTLConnectorNode.h"
#include "vtkMRMLAnnotationTextNode.h"
#include "vtkMRMLLinearTransformNode.h"
#include "vtkMRMLIGTLStatusNode.h"
#include "vtkMRMLIGTLSensorNode.h"
#include "vtkIGTLToMRMLString.h"
#include "vtkIGTLToMRMLStatus.h"
#include "vtkIGTLToMRMLSensor.h"
#include "vtkIGTLToMRMLTrajectory.h"

//------------------------------------------------------------------------------
vtkMRMLNodeNewMacro(vtkMRMLIGTLSessionManagerNode);

//----------------------------------------------------------------------------
vtkMRMLIGTLSessionManagerNode::vtkMRMLIGTLSessionManagerNode()
{
  this->ConnectorNodeIDInternal = 0;
  this->ConnectorNodeReferenceRole = 0;
  this->ConnectorNodeReferenceMRMLAttributeName = 0;
  this->MessageNodeReferenceRole = 0;
  this->MessageNodeReferenceMRMLAttributeName = 0;
  this->DrivingSensorNodeReferenceRole = 0;
  this->DrivingSensorNodeReferenceMRMLAttributeName = 0;

  this->SetConnectorNodeReferenceRole("connector");
  this->SetConnectorNodeReferenceMRMLAttributeName("connectorNodeRef");

  this->SetMessageNodeReferenceRole("message");
  this->SetMessageNodeReferenceMRMLAttributeName("messageNodeRef");

  this->SetDrivingSensorNodeReferenceRole("drivingSensor");
  this->SetDrivingSensorNodeReferenceMRMLAttributeName("drivingSensorRef");

  this->HideFromEditors = 0;
  this->AddNodeReferenceRole(this->GetConnectorNodeReferenceRole(),
                             this->GetConnectorNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetMessageNodeReferenceRole(),
                             this->GetMessageNodeReferenceMRMLAttributeName());
  this->AddNodeReferenceRole(this->GetDrivingSensorNodeReferenceRole(),
			     this->GetDrivingSensorNodeReferenceMRMLAttributeName());

  this->StringMessageConverter     = NULL;
  this->StatusMessageConverter     = NULL;
  this->SensorMessageConverter     = NULL;
  this->TrajectoryMessageConverter = NULL;

  this->OutQueryNodeIDInternal = NULL;

  this->InStatusNodeIDInternal = NULL;
  this->InCurrentSensorNodeIDInternal = NULL;
  this->InTargetSensorNodeIDInternal = NULL;
  this->InCurrentPositionTransformNodeIDInternal = NULL;
  this->InActualTargetTransformNodeIDInternal    = NULL;

  this->InStartUpStatusNodeIDInternal            = NULL;
  this->InZeroingStatusNodeIDInternal            = NULL;
  this->InCalibrationStatusNodeIDInternal        = NULL;
  this->InTargetingStatusNodeIDInternal          = NULL;
  this->InMoveToStatusNodeIDInternal             = NULL;
  this->InHoldStatusNodeIDInternal               = NULL;
  this->InEmergencyStatusNodeIDInternal          = NULL;

  this->QueryCounter = 0;

  this->RegistrationMatrix = NULL;
}

//----------------------------------------------------------------------------
vtkMRMLIGTLSessionManagerNode::~vtkMRMLIGTLSessionManagerNode()
{
}

//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::WriteXML(ostream& of, int nIndent)
{
  Superclass::WriteXML(of, nIndent);
}

//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::ReadXMLAttributes(const char** atts)
{
  int disabledModify = this->StartModify();

  Superclass::ReadXMLAttributes(atts);

  this->EndModify(disabledModify);
}


//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::PrintSelf(ostream& os, vtkIndent indent)
{
  Superclass::PrintSelf(os,indent);
  const char* connectorNodeID = this->GetNodeReferenceID(this->GetConnectorNodeReferenceRole());

  os << indent << "ConnectorNodeID: " <<
    (connectorNodeID ? connectorNodeID : "(none)") << "\n";
}


//----------------------------------------------------------------------------
const char* vtkMRMLIGTLSessionManagerNode::GetConnectorNodeID()
{
  return this->GetConnectorNodeIDInternal();
}

//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::SetAndObserveConnectorNodeID(const char *connectorNodeID)
{
  // ------------------------------------------------------------
  // Set reference node to start observing events

  if (connectorNodeID == 0 && this->GetNodeReferenceID(this->GetConnectorNodeReferenceRole()) == 0)
    {
    // was NULL and still NULL, nothing to do
    return;
    }

  if (connectorNodeID != 0 && this->GetNodeReferenceID(this->GetConnectorNodeReferenceRole()) != 0 &&
      !strcmp(connectorNodeID, this->GetNodeReferenceID(this->GetConnectorNodeReferenceRole())))
    {
    //the same ID, nothing to do
    return;
    }

  vtkMRMLScene * scene = this->GetScene();
  if (!scene)
    {
    return;
    }

  vtkMRMLIGTLConnectorNode* cnode =
    vtkMRMLIGTLConnectorNode::SafeDownCast(scene->GetNodeByID(connectorNodeID));

  if (!cnode) // There is no connector node with the specified MRML ID
    {
    return;
    }

  // use vtkMRMLNode call to set and observe reference
  vtkSmartPointer< vtkIntArray > events = vtkSmartPointer< vtkIntArray >::New();
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::ConnectedEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::DisconnectedEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::ActivatedEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::DeactivatedEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::ReceiveEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::NewDeviceEvent);
  events->InsertNextValue(vtkMRMLIGTLConnectorNode::DeviceModifiedEvent);
  events->InsertNextValue(vtkCommand::ModifiedEvent);
  this->SetAndObserveNodeReferenceID(this->GetConnectorNodeReferenceRole(),
                                     connectorNodeID, events);
  //this->InvokeEvent(vtkMRMLIGTLSessionManagerNode::TransformModifiedEvent, NULL);
  this->SetConnectorNodeIDInternal(connectorNodeID);

  // ------------------------------------------------------------
  // Remove outgoing message nodes currently registered.

  while (cnode->GetNumberOfOutgoingMRMLNodes() > 0)
    {
    vtkMRMLNode* node = cnode->GetOutgoingMRMLNode(0);
    const char* attr = node->GetAttribute("IGTLSessionManager.Created");
    if (strcmp(attr, "1") == 0)
      {
      cnode->UnregisterOutgoingMRMLNode(node);
      scene->RemoveNode(node);
      }
    else
      {
      cnode->UnregisterOutgoingMRMLNode(node);
      }
    }

  // Remove incoming message nodes currently registered.
  while (cnode->GetNumberOfIncomingMRMLNodes() > 0)
    {
    vtkMRMLNode* node = cnode->GetIncomingMRMLNode(0);
    const char* attr = node->GetAttribute("IGTLSessionManager.Created");
    if (strcmp(attr, "1") == 0)
      {
      cnode->UnregisterIncomingMRMLNode(node);
      scene->RemoveNode(node);
      }
    else
      {
      cnode->UnregisterIncomingMRMLNode(node);
      }
    }

  // ------------------------------------------------------------
  // Register message converter

  vtkSmartPointer<vtkIGTLToMRMLString> mcString = 
    vtkSmartPointer<vtkIGTLToMRMLString>::New();
  if (!this->StringMessageConverter)
    {
    this->StringMessageConverter = mcString.GetPointer();
    }
  cnode->RegisterMessageConverter(this->StringMessageConverter);

  vtkSmartPointer<vtkIGTLToMRMLStatus> mcStatus =
    vtkSmartPointer<vtkIGTLToMRMLStatus>::New();
  if (!this->StatusMessageConverter)
    {
    this->StatusMessageConverter = mcStatus.GetPointer();
    }
  cnode->RegisterMessageConverter(this->StatusMessageConverter);

  vtkSmartPointer<vtkIGTLToMRMLSensor> mcSensor =
    vtkSmartPointer<vtkIGTLToMRMLSensor>::New();
  if (!this->SensorMessageConverter)
    {
    this->SensorMessageConverter = mcSensor.GetPointer();
    }
  cnode->RegisterMessageConverter(this->SensorMessageConverter);

  vtkSmartPointer<vtkIGTLToMRMLTrajectory> mcTrajectory =
    vtkSmartPointer<vtkIGTLToMRMLTrajectory>::New();
  if (!this->TrajectoryMessageConverter)
    {
    this->TrajectoryMessageConverter = mcTrajectory.GetPointer();
    }
  cnode->RegisterMessageConverter(this->TrajectoryMessageConverter);


  // ------------------------------------------------------------
  // Register message nodes

  this->RegisterMessageNodes(cnode);
}


//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::AddAndObserveMessageNodeID(const char *messageNodeID)
{
  int n = this->GetNumberOfNodeReferences(this->GetMessageNodeReferenceRole());
  for (int i = 0; i < n; i ++)
    {
    const char* id = GetNthNodeReferenceID(this->GetMessageNodeReferenceRole(), i);
    if (strcmp(messageNodeID, id) == 0)
      {
      // Alredy on the list. Remove it.
      return;
      }
    }

  vtkSmartPointer< vtkIntArray > events = vtkSmartPointer< vtkIntArray >::New();
  events->InsertNextValue(vtkCommand::ModifiedEvent);
  this->AddAndObserveNodeReferenceID(this->GetMessageNodeReferenceRole(), messageNodeID, events);

  //this->InvokeEvent(vtkMRMLIGTLSessionManagerNode::TransformModifiedEvent, NULL);
}


//---------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::ProcessMRMLEvents ( vtkObject *caller,
                                                        unsigned long event,
                                                        void *vtkNotUsed(callData) )
{
  // as retrieving the parent transform node can be costly (browse the scene)
  // do some checks here to prevent retrieving the node for nothing.
  if (caller == NULL)
    {
    return;
    }

  vtkMRMLNode * node = vtkMRMLNode::SafeDownCast(caller);
  if (!node)
    {
    return;
    }

  // Check connector event
  if (this->GetConnectorNodeIDInternal() &&
      strcmp(node->GetID(), this->GetConnectorNodeIDInternal()) == 0)
    {
    vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
    if (cnode && event == vtkMRMLIGTLConnectorNode::NewDeviceEvent)
      {
      int ni = cnode->GetNumberOfIncomingMRMLNodes();
      for (int i = 0; i < ni; i ++)
        {
        vtkMRMLNode* inode = cnode->GetIncomingMRMLNode(i);
        if (inode)
          {
          std::string rcvPrefix;
          std::string rcvID;
          // Check if the node name includes '_'.
          if (ParseMessageName(inode->GetName(), rcvPrefix, rcvID))
            {
            if (rcvPrefix.compare("ACK_") == 0)
              {
              this->OnCommandAcknowledged(rcvID.c_str(), inode);
              }
            }
          }
        }
      }
    return;
    }

  else if (event == vtkCommand::ModifiedEvent)
    {
    // Transform message
    vtkMRMLLinearTransformNode* tnode =
      vtkMRMLLinearTransformNode::SafeDownCast(node);
    if (tnode)
      {
      if (this->GetInCurrentPositionTransformNodeIDInternal() &&
          strcmp(node->GetID(), this->GetInCurrentPositionTransformNodeIDInternal()) == 0)
        {
        this->OnPositionMessageReceived(tnode);
        }
      else if (this->GetInActualTargetTransformNodeIDInternal()&&
               strcmp(node->GetID(), this->GetInActualTargetTransformNodeIDInternal()) == 0)
        {
        this->OnActualTargetMessageReceived(tnode);
        }
      return;
      }

    // Status message
    vtkMRMLIGTLStatusNode* snode =
      vtkMRMLIGTLStatusNode::SafeDownCast(node);
    if (snode)
      {
      if (this->GetInStatusNodeIDInternal() &&
          strcmp(node->GetID(), this->GetInStatusNodeIDInternal()) == 0)
        {
        this->OnStateMessageReceived(snode);
        }
      else if (this->GetInStartUpStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInStartUpStatusNodeIDInternal()) == 0)
        {
        this->OnStartUpStatusNodeUpdated(snode);
        }
      else if (this->GetInZeroingStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInZeroingStatusNodeIDInternal()) == 0)
        {
        this->OnZeroingStatusNodeUpdated(snode);
        }
      else if (this->GetInCalibrationStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInCalibrationStatusNodeIDInternal()) == 0)
        {
        this->OnCalibrationStatusNodeUpdated(snode);
        }
      else if (this->GetInTargetingStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInTargetingStatusNodeIDInternal()) == 0)
        {
        this->OnTargetingStatusNodeUpdated(snode);
        }
      else if (this->GetInMoveToStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInMoveToStatusNodeIDInternal()) == 0)
        {
        this->OnMoveToStatusNodeUpdated(snode);
        }
      else if (this->GetInHoldStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInHoldStatusNodeIDInternal()) == 0)
        {
        this->OnHoldStatusNodeUpdated(snode);
        }
      else if (this->GetInEmergencyStatusNodeIDInternal() &&
               strcmp(node->GetID(), this->GetInEmergencyStatusNodeIDInternal()) == 0)
        {
        this->OnEmergencyStatusNodeUpdated(snode);
        }
      return;
      }
    }

  // Sensor message
  vtkMRMLIGTLSensorNode* sensorNode =
    vtkMRMLIGTLSensorNode::SafeDownCast(node);
  if (sensorNode)
    {
      if (this->GetInCurrentSensorNodeIDInternal() && this->GetInTargetSensorNodeIDInternal())
	{
	this->OnSensorMessageReceived(sensorNode);
	}
    }

}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::RequestStateTransition(int state)
{
  const char *StatusString[] = {
    "NONE",
    "START_UP",
    "ZEROING",
    "CALIBRATION",
    "TARGETING",
    "MOVETO",
    "HOLD",
    "EMERGENCY",
  };

  if (state <= 0 || state >= NUM_STATES)
    {
    return -1;
    }

  return this->SendCommandWithQueryID(StatusString[state]);
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::RequestCurrentState()
{
  // Check connection
  vtkMRMLIGTLConnectorNode * cnode = this->GetActiveConnector();
  if (!cnode)
    {
    return 0;
    }

  vtkMRMLNode * node = this->GetScene()->GetNodeByID(this->GetOutQueryNodeIDInternal());
  vtkMRMLIGTLQueryNode * qnode = vtkMRMLIGTLQueryNode::SafeDownCast(node);
  if (!qnode)
    {
    std::cerr << "vtkMRMLIGTLSessionManagerNode::RequestCurrentState():" << std::endl;
    std::cerr << "QueryNode not found." << std::endl;
    return -1;
    }

  qnode->SetName("STATE");
  qnode->SetIGTLName("GET_STATUS");
  qnode->Modified();

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::RequestCurrentPosition()
{
  // Check connection
  vtkMRMLIGTLConnectorNode * cnode = this->GetActiveConnector();
  if (!cnode)
    {
    return 0;
    }


  // Generate a message name with a unique query ID.
  this->QueryCounter ++;
  int qid = this->QueryCounter;
  std::stringstream ss;
  ss << "POS_" << std::setw(4) << std::setfill('0') << qid;

  vtkMRMLNode * node = this->GetScene()->GetNodeByID(this->GetOutQueryNodeIDInternal());
  vtkMRMLIGTLQueryNode * qnode = vtkMRMLIGTLQueryNode::SafeDownCast(node);
  if (!qnode)
    {
    std::cerr << "vtkMRMLIGTLSessionManagerNode::RequestCurrentState():" << std::endl;
    std::cerr << "QueryNode not found." << std::endl;
    return -1;
    }

  qnode->SetName(ss.str().c_str());
  qnode->SetIGTLName("GET_TRANS");
  qnode->Modified();

  return qid;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::RequestCurrentEncoderValues()
{
  // Generate a message name with a unique query ID.
  this->QueryCounter ++;
  int qid = this->QueryCounter;
  std::stringstream ss;
  ss << "ENC_" << std::setw(4) << std::setfill('0') << qid;

  vtkMRMLNode * node = this->GetScene()->GetNodeByID(this->GetOutQueryNodeIDInternal());
  vtkMRMLIGTLQueryNode * qnode = vtkMRMLIGTLQueryNode::SafeDownCast(node);
  if (!qnode)
    {
    std::cerr << "vtkMRMLIGTLSessionManagerNode::RequestCurrentEncoderValues():" << std::endl;
    std::cerr << "QueryNode not found." << std::endl;
    return -1;
    }

  qnode->SetName(ss.str().c_str());
  qnode->SetIGTLName("SENSOR");
  qnode->Modified();

  return qid;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::SendCalibrationTransform(vtkMatrix4x4* transform)
{

  // Check connection
  vtkMRMLIGTLConnectorNode * cnode = this->GetActiveConnector();
  if (!cnode)
    {
    return 0;
    }

  // Generate a message name with a unique query ID.
  this->QueryCounter ++;
  int qid = this->QueryCounter;
  std::stringstream ss;
  ss << "CLB_" << std::setw(4) << std::setfill('0') << qid;

  // Create a message
  vtkSmartPointer< vtkMRMLLinearTransformNode > tnode =
    vtkSmartPointer< vtkMRMLLinearTransformNode >::New();
  tnode->GetMatrixTransformToParent()->DeepCopy(transform);
  tnode->SetName(ss.str().c_str());
  this->ConfigureMessageNode(cnode, tnode, vtkMRMLIGTLConnectorNode::IO_OUTGOING);

  // Update the node to send the message.
  tnode->InvokeEvent(vtkMRMLTransformableNode::TransformModifiedEvent);

  return qid;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::SendTarget(vtkMatrix4x4* target)
{

  // Check connection
  vtkMRMLIGTLConnectorNode * cnode = this->GetActiveConnector();
  if (!cnode)
    {
    return 0;
    }

  // Generate a message name with a unique query ID.
  this->QueryCounter ++;
  int qid = this->QueryCounter;
  std::stringstream ss;
  ss << "TGT_" << std::setw(4) << std::setfill('0') << qid;

  // Create a message
  vtkSmartPointer< vtkMRMLLinearTransformNode > tnode =
    vtkSmartPointer< vtkMRMLLinearTransformNode >::New();
  tnode->GetMatrixTransformToParent()->DeepCopy(target);
  tnode->SetName(ss.str().c_str());
  this->ConfigureMessageNode(cnode, tnode, vtkMRMLIGTLConnectorNode::IO_OUTGOING);

  // Update the node to send the message.
  tnode->InvokeEvent(vtkMRMLTransformableNode::TransformModifiedEvent);

  return qid;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::SendTarget(double r, double a, double s)
{
  vtkSmartPointer<vtkMatrix4x4> target = vtkSmartPointer<vtkMatrix4x4>::New();
  target->Identity();
  target->SetElement(0, 3, r);
  target->SetElement(1, 3, a);
  target->SetElement(2, 3, s);

  return this->SendTarget(target);
}

//----------------------------------------------------------------------------
const char * vtkMRMLIGTLSessionManagerNode::GetStateString(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return "";
    }
  else if (strcmp(snode->GetErrorName(), "START_UP") == 0)
    {
    return "Start up";
    }
  else if (strcmp(snode->GetErrorName(), "ZEROING") == 0)
    {
    return "Zeroing";
    }
  else if (strcmp(snode->GetErrorName(), "CALIBRATION") == 0)
    {
    return "Calibration";
    }
  else if (strcmp(snode->GetErrorName(), "TARGETING") == 0)
    {
    return "Targeting";
    }
  else if (strcmp(snode->GetErrorName(), "MOVETO") == 0)
    {
    return "Move To";
    }
  else if (strcmp(snode->GetErrorName(), "HOLD") == 0)
    {
    return "Hold";
    }
  else if (strcmp(snode->GetErrorName(), "EMERGENCY") == 0)
    {
    return "Emergency";
    }

  return NULL;
}


//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::OnNodeReferenceAdded(vtkMRMLNodeReference *reference)
{
  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    {
    return;
    }

  vtkMRMLNode* node = scene->GetNodeByID(reference->GetReferencedNodeID());
  if (!node)
    {
    return;
    }

  if (strcmp(reference->GetReferenceRole(), this->GetMessageNodeReferenceRole()) == 0)
    {
    }
  else if (strcmp(reference->GetReferenceRole(), this->GetConnectorNodeReferenceRole()) == 0)
    {
    }
}


//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::OnNodeReferenceRemoved(vtkMRMLNodeReference *reference)
{
  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    {
    return;
    }

  vtkMRMLNode* node = scene->GetNodeByID(reference->GetReferencedNodeID());
  if (!node)
    {
    return;
    }

  if (strcmp(reference->GetReferenceRole(), this->GetMessageNodeReferenceRole()) == 0)
    {
    }
  else if (strcmp(reference->GetReferenceRole(), this->GetConnectorNodeReferenceRole()) == 0)
    {
    }
}


//----------------------------------------------------------------------------
void vtkMRMLIGTLSessionManagerNode::OnNodeReferenceModified(vtkMRMLNodeReference *reference)
{
  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    {
    return;
    }

  vtkMRMLNode* node = scene->GetNodeByID(reference->GetReferencedNodeID());
  if (!node)
    {
    return;
    }

  if (strcmp(reference->GetReferenceRole(), this->GetMessageNodeReferenceRole()) == 0)
    {
    }
  else if (strcmp(reference->GetReferenceRole(), this->GetConnectorNodeReferenceRole()) == 0)
    {
    }
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnCommandAcknowledged(const char* rcvID, vtkMRMLNode* inode)
{

  vtkMRMLNode* node = this->GetScene()->GetNodeByID(this->GetConnectorNodeIDInternal());
  vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);

  if (!cnode)
    {
    return 0;
    }

  int no = cnode->GetNumberOfOutgoingMRMLNodes();
  for (int j = 0; j < no; j ++)
    {
    std::string sndPrefix;
    std::string sndID;

    vtkMRMLNode* onode = cnode->GetOutgoingMRMLNode(j);
    if (!onode)
      {
      continue;
      }
    ParseMessageName(onode->GetName(), sndPrefix, sndID);
    if (strcmp(rcvID, sndID.c_str()) == 0)
      {
      int acknowledged = 0;

      if (sndPrefix.compare("CMD_") == 0)
        {
        // Pending commnd is acknowledged
        vtkMRMLAnnotationTextNode * atnode = vtkMRMLAnnotationTextNode::SafeDownCast(inode);
        if (atnode)
          {
          acknowledged = 1;
          }
        }
      else if (sndPrefix.compare("CLB_") == 0)
        {
        // Pending calibration request is acknowledged
        vtkMRMLLinearTransformNode * tnode =
          vtkMRMLLinearTransformNode::SafeDownCast(inode);
        if (tnode)
          {
          // TODO: check the matrix
          acknowledged = 1;
          }
        }
      else if (sndPrefix.compare("TGT_") == 0)
        {
        // Target command is received.
        // Pending calibration request is acknowledged
        vtkMRMLLinearTransformNode * tnode =
          vtkMRMLLinearTransformNode::SafeDownCast(inode);
        if (tnode)
          {
          // TODO: check the matrix
          acknowledged = 1;
          }
        }
      if (acknowledged)
        {
        cnode->UnregisterOutgoingMRMLNode(onode);
        cnode->UnregisterIncomingMRMLNode(inode);
        this->GetScene()->RemoveNode(onode);
        this->GetScene()->RemoveNode(inode);
        this->InvokeEvent(MessageAcknowledgedEvent, (void*)rcvID);
        return 1;
        }
      }
    }
  return 0;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnStateMessageReceived(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  const char *StatusString[] = {
    "NONE",
    "START_UP",
    "ZEROING",
    "CALIBRATION",
    "TARGETING",
    "MOVETO",
    "HOLD",
    "EMERGENCY",
  };

  const char* mname = snode->GetErrorName();

  for (int i = 0; i < NUM_STATES; i ++)
    {
    if (strcmp(mname, StatusString[i]) == 0)
      {
      // TODO: Check the code in status message.
      if (this->GetCurrentState() != i)
        {
        this->SetCurrentState(i);
        // TODO: If there is error, add it to the message in the status node.
        DeviceStatusMessage dsm;
        dsm.CurrentState = mname;
        dsm.Message = "Status changed.";
        dsm.Description = "";
        dsm.Type = STATE_MESSAGE;
        this->InvokeEvent(StateChangeEvent, &dsm);

        // If CALIBRATION transition succeed, send calibration matrix
        if (this->GetCurrentState() == STATE_CALIBRATION &&
            snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK &&
            this->RegistrationMatrix)
          {
          this->SendCalibrationTransform(this->RegistrationMatrix);
          }

        return 1;
        }
      }
    }

  return 0;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnSensorMessageReceived(vtkMRMLIGTLSensorNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  if (strcmp(snode->GetName(), "ENCODER") == 0)
    {
    this->InvokeEvent(EncoderValuesUpdatedEvent, (void*)snode);
    }
  else if (strcmp(snode->GetName(), "TARGET_ENC") == 0)
    {
    this->InvokeEvent(EncoderTargetValuesUpdatedEvent, (void*)snode);
    }

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnPositionMessageReceived(vtkMRMLLinearTransformNode* tnode)
{
  if (!tnode)
    {
    return 0;
    }

  // TODO: check the transform
  this->InvokeEvent(CurrentPositionUpdatedEvent, (void*)tnode);

  return 1;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnActualTargetMessageReceived(vtkMRMLLinearTransformNode* tnode)
{
  if (!tnode)
    {
    return 0;
    }

  // TODO: check the transform
  this->InvokeEvent(ActualTargetUpdatedEvent, (void*)tnode);

  return 1;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnStartUpStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "START_UP";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "START_UP completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in START_UP process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnZeroingStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "ZEROING";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "ZEROING completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in ZEROING process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnCalibrationStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "CALIBRATION";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    this->InvokeEvent(CalibrationSucceedEvent);
    dsm.Message = "CALIBRATION completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_CONFIG_ERROR)
    {
    dsm.Message = "Calibration matrix received is invalid";
    dsm.Type = ERROR_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in CALIBRATION process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;

}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnTargetingStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "TARGETING";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "TARGETING completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_CONFIG_ERROR)
    {
    dsm.Message = "Target received is invalid (out of range, ...)";
    dsm.Type = ERROR_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in TARGETING process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnMoveToStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "MOVETO";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "MOVETO completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in MOVETO process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnHoldStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "HOLD";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "HOLD completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in HOLD process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  return 1;
}

//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::OnEmergencyStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode)
{
  if (!snode)
    {
    return 0;
    }

  // TODO: If there is error, add it to the StatusString in the status node.
  DeviceStatusMessage dsm;
  dsm.CurrentState = "EMERGENCY";
  if (snode->GetCode() == vtkMRMLIGTLStatusNode::STATUS_OK)
    {
    dsm.Message = "EMERGENCY completed successfully.";
    dsm.Type = STATUS_MESSAGE;
    }
  else
    {
    std::stringstream ssMessage;
    ssMessage << "Error in EMERGENCY process (" << snode->GetCode() << ")";
    dsm.Message =  ssMessage.str();
    dsm.Type = ERROR_MESSAGE;
    }
  dsm.Description = snode->GetStatusString();
  this->InvokeEvent(DeviceStatusEvent, &dsm);

  // TODO: If there is error, add it to the StatusString in the status node.
  return 1;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::ConfigureMessageNode(vtkMRMLIGTLConnectorNode* cnode, vtkMRMLNode* node, int io)
{
  if (!cnode || !node)
    {
    return 0;
    }

  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    {
    return 0;
    }

  node->SetAttribute("IGTLSessionManager.Created", "1");
  scene->AddNode(node);

  if (io == vtkMRMLIGTLConnectorNode::IO_OUTGOING)
    {
    cnode->RegisterOutgoingMRMLNode(node);
    }
  else // vtkMRMLIGTLConnectorNode::IO_INCOMING
    {
    cnode->RegisterIncomingMRMLNode(node);
    }

  this->AddAndObserveMessageNodeID(node->GetID());

  return 1;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::RegisterMessageNodes(vtkMRMLIGTLConnectorNode* cnode)
{

  if (!cnode)
    {
    return 0;
    }

  // --------------------------------------------------
  // Outgoing nodes
  vtkSmartPointer< vtkMRMLIGTLQueryNode > query =
    vtkSmartPointer<vtkMRMLIGTLQueryNode>::New();
  query->SetName("STATE");
  query->SetIGTLName("STATUS");
  this->ConfigureMessageNode(cnode, query,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetOutQueryNodeIDInternal(query->GetID());


  // --------------------------------------------------
  // Incoming nodes

  vtkSmartPointer< vtkMRMLIGTLStatusNode > status =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  status->SetName("STATE");
  this->ConfigureMessageNode(cnode, status,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInStatusNodeIDInternal(status->GetID());

  vtkSmartPointer< vtkMRMLLinearTransformNode > actualTarget =
    vtkSmartPointer< vtkMRMLLinearTransformNode >::New();
  actualTarget->SetName("TARGET");
  this->ConfigureMessageNode(cnode, actualTarget,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInActualTargetTransformNodeIDInternal(actualTarget->GetID());

  vtkSmartPointer< vtkMRMLLinearTransformNode > currentPosition =
    vtkSmartPointer< vtkMRMLLinearTransformNode >::New();
  currentPosition->SetName("POSITION");
  this->ConfigureMessageNode(cnode, currentPosition,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInCurrentPositionTransformNodeIDInternal(currentPosition->GetID());

  // Sensor command
  vtkSmartPointer< vtkMRMLIGTLSensorNode > currentSensorMessage =
    vtkSmartPointer< vtkMRMLIGTLSensorNode >::New();
  currentSensorMessage->SetName("ENCODER");
  this->ConfigureMessageNode(cnode, currentSensorMessage,
			     vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInCurrentSensorNodeIDInternal(currentSensorMessage->GetID());
  this->AddNodeReferenceID(this->GetDrivingSensorNodeReferenceRole(), currentSensorMessage->GetID());

  vtkSmartPointer< vtkMRMLIGTLSensorNode > targetSensorMessage =
    vtkSmartPointer< vtkMRMLIGTLSensorNode >::New();
  targetSensorMessage->SetName("TARGET_ENC");
  this->ConfigureMessageNode(cnode, targetSensorMessage,
			     vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInTargetSensorNodeIDInternal(targetSensorMessage->GetID());

  // Status commands
  vtkSmartPointer< vtkMRMLIGTLStatusNode > startUpStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  startUpStatus->SetName("START_UP");
  this->ConfigureMessageNode(cnode, startUpStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInStartUpStatusNodeIDInternal(startUpStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > zeroingStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  zeroingStatus->SetName("ZEROING");
  this->ConfigureMessageNode(cnode, zeroingStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInZeroingStatusNodeIDInternal(zeroingStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > calibrationStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  calibrationStatus->SetName("CALIBRATION");
  this->ConfigureMessageNode(cnode, calibrationStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInCalibrationStatusNodeIDInternal(calibrationStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > targetingStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  targetingStatus->SetName("TARGETING");
  this->ConfigureMessageNode(cnode, targetingStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInTargetingStatusNodeIDInternal(targetingStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > moveToStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  moveToStatus->SetName("MOVETO");
  this->ConfigureMessageNode(cnode, moveToStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInMoveToStatusNodeIDInternal(moveToStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > holdStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  holdStatus->SetName("HOLD");
  this->ConfigureMessageNode(cnode, holdStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInHoldStatusNodeIDInternal(holdStatus->GetID());

  vtkSmartPointer< vtkMRMLIGTLStatusNode > emergencyStatus =
    vtkSmartPointer< vtkMRMLIGTLStatusNode >::New();
  emergencyStatus->SetName("EMERGENCY");
  this->ConfigureMessageNode(cnode, emergencyStatus,
                             vtkMRMLIGTLConnectorNode::IO_INCOMING);
  this->SetInEmergencyStatusNodeIDInternal(emergencyStatus->GetID());

  return 1;
}


//----------------------------------------------------------------------------
vtkMRMLIGTLConnectorNode* vtkMRMLIGTLSessionManagerNode::GetActiveConnector()
{
  vtkMRMLScene* scene = this->GetScene();
  if (!scene)
    {
    return NULL;
    }

  vtkMRMLNode* node = scene->GetNodeByID(this->GetConnectorNodeIDInternal());
  vtkMRMLIGTLConnectorNode* cnode = vtkMRMLIGTLConnectorNode::SafeDownCast(node);
  if (!cnode)
    {
    return NULL;
    }

  if (cnode->GetState() == vtkMRMLIGTLConnectorNode::STATE_CONNECTED)
    {
    return cnode;
    }
  else
    {
    return NULL;
    }
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::SetErrorType(int errType)
{
  this->ErrorType = errType;
  this->InvokeEvent(DeviceErrorEvent);
  return 1;
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::ParseMessageName(std::string messageName, std::string& prefix, std::string& id)
{
  // Split the message name into prefix (e.g. "CMD_") and ID.

  std::size_t f;
  f = messageName.find('_');
  if (f!=std::string::npos)
    {
    prefix = messageName.substr(0, f+1);
    id = messageName.substr(f+1);
    return 1;
    }
  else
    {
    prefix = "";
    id = "";
    return 0;
    }
}


//----------------------------------------------------------------------------
int vtkMRMLIGTLSessionManagerNode::SendCommandWithQueryID(const char* command)
{
  // Check connection
  vtkMRMLIGTLConnectorNode * cnode = this->GetActiveConnector();
  if (!cnode)
    {
    return 0;
    }

  // Generate a message name with a unique query ID.
  this->QueryCounter ++;
  int qid = this->QueryCounter;
  std::stringstream ss;
  ss << "CMD_" << std::setw(4) << std::setfill('0') << qid;

  // Create a message
  vtkSmartPointer< vtkMRMLAnnotationTextNode > outCommand = vtkSmartPointer< vtkMRMLAnnotationTextNode >::New();
  outCommand->SetName(ss.str().c_str());
  this->ConfigureMessageNode(cnode, outCommand, vtkMRMLIGTLConnectorNode::IO_OUTGOING);

  // Update the node to send the message.
  outCommand->SetTextLabel(command);

  return qid;
}
