#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <string>
#include <tf/tf.h>
#include "waypoint.hpp"
#include <iostream>

bra::Waypoint::Waypoint(std::string robotName): _robotName(robotName){

    _client = new MoveBaseClient(_robotName + "_pc/move_base", true);

    while(!_client->waitForServer(ros::Duration(5.0))){
        ROS_INFO("Waiting for the move_base action server to come up.");
    }
    ROS_INFO("Move Base Actived.");
}

bool bra::Waypoint::run(float x, float y, float th){
    move_base_msgs::MoveBaseGoal poseGoal;
    poseGoal.target_pose.header.frame_id = (_robotName + "_pc/odom");
    poseGoal.target_pose.header.stamp = ros::Time::now();

    poseGoal.target_pose.pose.position.x = x;
    poseGoal.target_pose.pose.position.y = y;
    poseGoal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(th);
    ROS_INFO("Sending goal");
    _client->sendGoal(poseGoal);
    _client->waitForResult();
    if(_client->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
        return true;
    else
       return false;
}