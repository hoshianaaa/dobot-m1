#include "ros/ros.h"
#include "DobotDll.h"

int main(int argc, char**argv)
{
    std::string deviceName;
    std::string deviceIP;

    ros::param::param<std::string>("/dobby/deviceName",deviceName,"Dobby");
    ros::param::param<std::string>("/dobby/deviceIP",deviceIP,"192.168.1.192");

    uint64_t queuedCmdIndex;

    // Connect to Dobot 
    if (ConnectDobot(deviceIP.c_str(), 115200, 0, 0) == DobotCommunicate_NoError) {
        ROS_INFO("Opened Dobot Communication");
    } else {
        ROS_ERROR("Invalid port name or Dobot is occupied by other application or something else :)");
    }
    // Set Commuinication timeout in ms 
    uint32_t timeout=20;
    SetCmdTimeout(timeout);

    // Set some Parameter and Initialisation
    // First clear the command queue
    if (SetQueuedCmdClear() == DobotCommunicate_NoError) {
           ROS_INFO("CMD Queue Cleared");
    }
    // and start the exec queue
    if (SetQueuedCmdStartExec() == DobotCommunicate_NoError) {
           ROS_INFO("CMD Queue Started");
    }

    // Set Coord/Jump/Accl/Vel/Joint Parameters
    PTPCoordinateParams coordparams;
    coordparams.xyzVelocity = 100; 
    coordparams.rVelocity = 100; 
    coordparams.xyzAcceleration = 100; 
    coordparams.rAcceleration = 100;
    if (SetPTPCoordinateParams(&coordparams, true, &queuedCmdIndex) == DobotCommunicate_NoError) {
      ROS_INFO("Dobot Coordinate Params Set.");
    }

    PTPJumpParams jumpparams;
    jumpparams.jumpHeight = 20;
    jumpparams.zLimit = 200;
    if (SetPTPJumpParams(&jumpparams, true, &queuedCmdIndex) == DobotCommunicate_NoError) {
      ROS_INFO("Dobot Junp Params Set.");
    }

    // Set Acc and Speed
    PTPCommonParams commonparams;
    commonparams.velocityRatio=100;
    commonparams.accelerationRatio=100;
    if (SetPTPCommonParams(&commonparams, true, &queuedCmdIndex) == DobotCommunicate_NoError) {
      ROS_INFO("Dobot Common Params Set.");
    }

    PTPJointParams jointparams;
    for (int i = 0; i < 4; i++) {
        jointparams.velocity[i] = 100;
        jointparams.acceleration[i] = 100;
    }

    if (SetPTPJointParams(&jointparams, true, &queuedCmdIndex) == DobotCommunicate_NoError) {
      ROS_INFO("Dobot Joint Params Set.");

    }

    // For fun - set Dobot M1 Name
    if (SetDeviceName(deviceName.c_str()) == DobotCommunicate_NoError) {
      ROS_INFO("Dobot is Running.");
    }
}
