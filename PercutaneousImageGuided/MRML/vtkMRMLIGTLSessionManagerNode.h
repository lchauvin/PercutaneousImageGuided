/*=auto=========================================================================

  Portions (c) Copyright 2005 Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Program:   3D Slicer

  =========================================================================auto=*/

#ifndef __vtkMRMLIGTLSessionManagerNode_h
#define __vtkMRMLIGTLSessionManagerNode_h

// MRML includes
#include "vtkMRMLNode.h"
#include "vtkMatrix4x4.h"
#include "vtkSlicerPercutaneousImageGuidedModuleMRMLExport.h"

class vtkMRMLLinearTransformNode;

class vtkIGTLToMRMLString;
class vtkIGTLToMRMLStatus;
class vtkIGTLToMRMLSensor;
class vtkIGTLToMRMLTrajectory;
class vtkMRMLIGTLConnectorNode;
class vtkMRMLIGTLStatusNode;
class vtkMRMLIGTLSensorNode;
class vtkMRMLAnnotationTextNode;
class vtkMRMLIGTLLinearTransformNode;


/// \brief MRML node to manage OpenIGTLink
///
/// Configure vtkMRMLIGTLConnectorNode and manage incoming and outgoing
/// data during a OpenIGTLink session.

class VTK_SLICER_PERCUTANEOUSIMAGEGUIDED_MODULE_MRML_EXPORT vtkMRMLIGTLSessionManagerNode : public vtkMRMLNode
{
 public:
  static vtkMRMLIGTLSessionManagerNode *New();

  vtkTypeMacro(vtkMRMLIGTLSessionManagerNode,vtkMRMLNode);

  void PrintSelf(ostream& os, vtkIndent indent);

  virtual vtkMRMLNode* CreateNodeInstance();

  ///
  /// Read node attributes from XML file
  virtual void ReadXMLAttributes( const char** atts);

  ///
  /// Write this node's information to a MRML file in XML format.
  virtual void WriteXML(ostream& of, int indent);

  ///
  /// Get node XML tag name (like Volume, Model)
  virtual const char* GetNodeTagName() {return "IGTLSessionManager";};

  ///
  /// Set and configure MRMLIGTLconnector node.
  /// Once the new connector node is set, RegisterMessageNodes() is called
  /// to register all message nodes used in the communication.
  void SetAndObserveConnectorNodeID(const char *connectorNodeID);

  ///
  /// Set a reference to transform node
  void AddAndObserveMessageNodeID(const char *transformNodeID);

  // Get the current status of the robot. This function only reads the state
  // stored in SessionManager. If the stored state needs to be updated,
  // call RequestCurrentState()
  vtkGetMacro(CurrentState, int);
  vtkSetMacro(CurrentState, int);

  ///
  /// Associated transform MRML node
  //vtkMRMLTransformNode* GetParentTransformNode();

  ///
  /// alternative method to propagate events generated in Transform nodes
  virtual void ProcessMRMLEvents ( vtkObject * /*caller*/,
                                   unsigned long /*event*/,
                                   void * /*callData*/ );

  //BTX
  virtual void OnNodeReferenceAdded(vtkMRMLNodeReference *reference);

  virtual void OnNodeReferenceRemoved(vtkMRMLNodeReference *reference);

  virtual void OnNodeReferenceModified(vtkMRMLNodeReference *reference);
  //ETX

  /// Get referenced connector node id
  const char *GetConnectorNodeID();

  /// Application specific API (will be moved to child class in the future)
  ///
  enum
  {
    StateChangeEvent                = 20000,
    MessageAcknowledgedEvent        = 20001,
    TimeOutEvent                    = 20002,
    DeviceErrorEvent                = 20003,
    CurrentPositionUpdatedEvent     = 20004,
    EncoderValuesUpdatedEvent       = 20005,
    EncoderTargetValuesUpdatedEvent = 20006,
    ActualTargetUpdatedEvent        = 20007,
    DeviceStatusEvent               = 20008,
    CalibrationSucceedEvent         = 20009,
    //NewMessageEvent,  // New messages other than status change
  };

  // Robot states
  enum
  {
    STATE_NONE = 0,
    STATE_STARTUP,
    STATE_ZEROING,
    STATE_CALIBRATION,
    STATE_TARGETING,
    STATE_MOVETO,
    STATE_HOLD,
    STATE_EMERGENCY,
    NUM_STATES,
  };

  enum
  {
    STATUS_MESSAGE=0,
    ERROR_MESSAGE,
    STATE_MESSAGE
  };

  typedef struct {
    std::string CurrentState;
    std::string Message;
    std::string Description;
    int         Type; // *_MESSAGE
  } DeviceStatusMessage;

  /*
    enum
    {
    COMSTATE_UNINITIALIZED = 0,
    COMSTATE_STARTUP_CMD_SENT,
    COMSTATE_STARTUP_CMD_ACK,
    COMSTATE_STARTUP_CMD_DONE, // SUCCESS
    COMSTATE_ZEROING_CMD_SENT,
    COMSTATE_ZEROING_CMD_ACK,
    COMSTATE_ZEROING_CMD_DONE,
    COMSTATE_CALIBRATION_CMD_SENT,
    COMSTATE_CALIBRATION_CMD_ACK,
    COMSTATE_CALIBRATION_TRANS_SENT,
    COMSTATE_CALIBRATION_TRANS_ACK,
    COMSTATE_CALIBRATION_TRANS_DONE, // SUCCESS
    COMSTATE_TARGETING_CMD_SENT,
    COMSTATE_TARGETING_CMD_ACK,
    COMSTATE_TARGETING_TARGET_SENT,
    COMSTATE_TARGETING_TARGET_ACK,
    COMSTATE_TARGETING_TARGET_DONE, // SUCCESS or ERROR (not ready or invalid target)
    COMSTATE_MOVETO_CMD_SENT,
    COMSTATE_MOVETO_CMD_ACK,
    COMSTATE_MOVETO_CMD_DONE, // SUCCESS
    COMSTATE_HOLD_CMD_SENT,
    COMSTATE_HOLD_CMD_ACK,
    COMSTATE_HOLD_CMD_DONE,
    COMSTATE_EMERGENCY_CMD_SENT,
    COMSTATE_EMERGENCY_CMD_ACK,
    COMSTATE_EMERGENCY_CMD_DONE,

    // TODO: following should not be used by CommunicationStatus?
    COMSTATE_GET_STATUS_SENT,
    COMSTATE_GET_CURRENT_POSITION_SENT,
    };
  */

  // Device errors
  enum
  {
    ERROR_TRANSITION_FAILED,
    ERROR_INITIALIZATION_STARTUP,
    ERROR_INITIALIZATION_ZEROING,
    ERROR_INITIALIZATION_CALIBRATION,
    ERROR_CALIBRATION_CONFIGURATION,
    ERROR_NOT_TARGETING_MODE,
    ERROR_TARGET_OUT_OF_RANGE,
    ERROR_MOVE_FAILED,
    ERROR_TARGET_NOT_REACHED,
    ERROR_FAILED_TO_HOLD
  };

 public:

  inline int GetError() { return this->ErrorType; };

  // Request the robot to transition to the specified state.
  // The function sends STRING("CMD_XXX") message and returns request ID.
  virtual int RequestStateTransition(int state);

  // Request the robot to send back its current state.
  // The function sends GET_STATUS("STATE") message and returns request ID.
  int RequestCurrentState();

  // Request the robot to transition to the specified state.
  // The function sends GET_TRANS("POS_XXX") message and returns request ID.
  virtual int RequestCurrentPosition();

  // Request the robot to send back current angle of its joints
  // The function sends GET_SENSOR("ENC_XXX") message and returns request ID.
  virtual int RequestCurrentEncoderValues();


  // Send a calibration matrix to the robot.
  // The function sends TRANSFORM("CLB_XXX") message and returns request ID.
  virtual int SendCalibrationTransform(vtkMatrix4x4* transform);

  // Send a target matrix to the robot.
  // The function sends TRANSFORM("TGT_XXX") message and returns request ID.
  virtual int SendTarget(vtkMatrix4x4* target);

  // Send a target matrix to the robot using RAS coordinates.
  // The function sends TRANSFORM("TGT_XXX") message and returns request ID.
  virtual int SendTarget(double r, double a, double s);

  // TODO:
  //virtual int  IsRequestPending(const char* requestID);
  //virtual void GetPendingRequests(std::list< std::string >);

  // Generate state string based on the status message received from the robot.
  virtual const char * GetStateString(vtkMRMLIGTLStatusNode* snode);;

  // Get the current position of the robot. This function only reads the position
  // stored in SessionManager. If the stored position needs to be updated,
  // call RequestCurrentPosition()
  //virtual void  GetCurrentPosition(vtkMatrix4x4* position);

  vtkGetObjectMacro(RegistrationMatrix, vtkMatrix4x4);
  vtkSetObjectMacro(RegistrationMatrix, vtkMatrix4x4);

  vtkSetStringMacro(DrivingSensorNodeReferenceRole);
  vtkGetStringMacro(DrivingSensorNodeReferenceRole);

  vtkSetStringMacro(DrivingSensorNodeReferenceMRMLAttributeName);
  vtkGetStringMacro(DrivingSensorNodeReferenceMRMLAttributeName);

 protected:
  vtkMRMLIGTLSessionManagerNode();
  ~vtkMRMLIGTLSessionManagerNode();
  vtkMRMLIGTLSessionManagerNode(const vtkMRMLIGTLSessionManagerNode&);
  void operator=(const vtkMRMLIGTLSessionManagerNode&);

  virtual int OnCommandAcknowledged(const char* rcvID, vtkMRMLNode* inode);
  virtual int OnStateMessageReceived(vtkMRMLIGTLStatusNode* snode);
  virtual int OnSensorMessageReceived(vtkMRMLIGTLSensorNode* snode);
  virtual int OnPositionMessageReceived(vtkMRMLLinearTransformNode* tnode);
  virtual int OnActualTargetMessageReceived(vtkMRMLLinearTransformNode* tnode);

  virtual int OnStartUpStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnZeroingStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnCalibrationStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnTargetingStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnMoveToStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnHoldStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);
  virtual int OnEmergencyStatusNodeUpdated(vtkMRMLIGTLStatusNode* snode);



  virtual int SendCommandWithQueryID(const char* command);

  /// Add a message node to the MRML scene and register to the connector node.
  /// This is a utility function for RegisterMessageNodes().
  /// io must be either vtkMRMLIGTLConnectorNode::IO_OUTGOING or
  /// vtkMRMLIGTLConnectorNode::IO_INCOMING
  int ConfigureMessageNode(vtkMRMLIGTLConnectorNode* cnode, vtkMRMLNode* node, int io);

  /// Register message nodes to the converter node
  /// This function will be called whenever a new connector node is set.
  /// This function has to be implemented per application.
  virtual int RegisterMessageNodes(vtkMRMLIGTLConnectorNode* cnode);

  /// Check connection status of the OpenIGTLink connector.
  /// Return a pointer to the connector node value if it is connected,
  /// otherwise return 0.
  vtkMRMLIGTLConnectorNode* GetActiveConnector();

  int SetRobotState(int newState);
  int SetErrorType(int errType);

  int ParseMessageName(std::string messageName, std::string& prefix, std::string& id);

  /// Incoming message handler
  //int ProcAcknowledgeString(vtkMRMLAnnotationTextNode * node);
  //int ProcStatus(vtkMRMLIGTLStatusNode * node);
  //int ProcCurrentPositionTransform(vtkMRMLLinearTransformNode * node);

  //----------------------------------------------------------------
  // Reference role strings
  //----------------------------------------------------------------
  char* ConnectorNodeReferenceRole;
  char* ConnectorNodeReferenceMRMLAttributeName;

  vtkSetStringMacro(ConnectorNodeReferenceRole);
  vtkGetStringMacro(ConnectorNodeReferenceRole);

  vtkSetStringMacro(ConnectorNodeReferenceMRMLAttributeName);
  vtkGetStringMacro(ConnectorNodeReferenceMRMLAttributeName);

  char* MessageNodeReferenceRole;
  char* MessageNodeReferenceMRMLAttributeName;

  vtkSetStringMacro(MessageNodeReferenceRole);
  vtkGetStringMacro(MessageNodeReferenceRole);

  vtkSetStringMacro(MessageNodeReferenceMRMLAttributeName);
  vtkGetStringMacro(MessageNodeReferenceMRMLAttributeName);

  char* DrivingSensorNodeReferenceRole;
  char* DrivingSensorNodeReferenceMRMLAttributeName;
  
 private:

  // TODO: We could use reference mechanism to keep the node IDs
  // for incoming and outgoing data.

  // OpenIGTLink connector Node
  char* ConnectorNodeIDInternal;
  vtkSetStringMacro(ConnectorNodeIDInternal);
  vtkGetStringMacro(ConnectorNodeIDInternal);

  //// Outgoing message nodes
  char* OutQueryNodeIDInternal;
  vtkSetStringMacro(OutQueryNodeIDInternal);
  vtkGetStringMacro(OutQueryNodeIDInternal);

  //// Incoming message nodes
  char* InStatusNodeIDInternal;
  vtkSetStringMacro(InStatusNodeIDInternal);
  vtkGetStringMacro(InStatusNodeIDInternal);

  char* InCurrentSensorNodeIDInternal;
  vtkSetStringMacro(InCurrentSensorNodeIDInternal);
  vtkGetStringMacro(InCurrentSensorNodeIDInternal);

  char* InTargetSensorNodeIDInternal;
  vtkSetStringMacro(InTargetSensorNodeIDInternal);
  vtkGetStringMacro(InTargetSensorNodeIDInternal);

  char* InCurrentPositionTransformNodeIDInternal;
  vtkSetStringMacro(InCurrentPositionTransformNodeIDInternal);
  vtkGetStringMacro(InCurrentPositionTransformNodeIDInternal);

  char* InActualTargetTransformNodeIDInternal;
  vtkSetStringMacro(InActualTargetTransformNodeIDInternal);
  vtkGetStringMacro(InActualTargetTransformNodeIDInternal);

  char* InStartUpStatusNodeIDInternal;
  vtkSetStringMacro(InStartUpStatusNodeIDInternal);
  vtkGetStringMacro(InStartUpStatusNodeIDInternal);

  char* InZeroingStatusNodeIDInternal;
  vtkSetStringMacro(InZeroingStatusNodeIDInternal);
  vtkGetStringMacro(InZeroingStatusNodeIDInternal);

  char* InCalibrationStatusNodeIDInternal;
  vtkSetStringMacro(InCalibrationStatusNodeIDInternal);
  vtkGetStringMacro(InCalibrationStatusNodeIDInternal);

  char* InTargetingStatusNodeIDInternal;
  vtkSetStringMacro(InTargetingStatusNodeIDInternal);
  vtkGetStringMacro(InTargetingStatusNodeIDInternal);

  char* InMoveToStatusNodeIDInternal;
  vtkSetStringMacro(InMoveToStatusNodeIDInternal);
  vtkGetStringMacro(InMoveToStatusNodeIDInternal);

  char* InHoldStatusNodeIDInternal;
  vtkSetStringMacro(InHoldStatusNodeIDInternal);
  vtkGetStringMacro(InHoldStatusNodeIDInternal);

  char* InEmergencyStatusNodeIDInternal;
  vtkSetStringMacro(InEmergencyStatusNodeIDInternal);
  vtkGetStringMacro(InEmergencyStatusNodeIDInternal);


  vtkIGTLToMRMLString     * StringMessageConverter;
  vtkIGTLToMRMLStatus     * StatusMessageConverter;
  vtkIGTLToMRMLSensor     * SensorMessageConverter;
  vtkIGTLToMRMLTrajectory * TrajectoryMessageConverter;

  //int CommunicationStatus; // COMSTATE_*
  int ErrorType;
  int CurrentState;

  int QueryCounter; // Used to generate a unique query ID.

  vtkMatrix4x4* RegistrationMatrix;
};

#endif
