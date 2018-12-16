#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/tf.h>
#include "odom.hpp"
#include <string>
#include <iostream>

bra::Odom::Odom(std::string robotName, ros::NodeHandle Node): _robotName(robotName), _Node(Node){
    // Init Variables
    _velocity[LINEAR] = _velocity[ANGULAR] = 0;
    _position[X] = _position[Y] = _position[TH] = 0;
    _Time[CURRENT] = ros::Time::now();
    _Time[LAST] = ros::Time::now();
    _enable = false;
    // Init Subscriber
    _OdomSub = _Node.subscribe((_robotName + "_mbed/velocity").c_str(), 20, &Odom::callback, this);
    // Init Publisher
    _OdomPub = _Node.advertise<nav_msgs::Odometry>((_robotName + "_pc/odom").c_str(), 10);
    // Setup Transform
    _OdomTransform.header.frame_id  = (_robotName + "_pc/odom");
    _OdomTransform.child_frame_id   = (_robotName + "_pc/base_link");
    _OdomTransform.transform.translation.z = 0;
    // Setup Odometry Topic
    _OdomMsg.header.frame_id        = (_robotName + "_pc/odom");
    _OdomMsg.child_frame_id         = (_robotName + "_pc/base_link");
    _OdomMsg.pose.pose.position.z   = 0;
    _OdomMsg.twist.twist.linear.y = _OdomMsg.twist.twist.linear.z = _OdomMsg.twist.twist.angular.x = 
                                                                                _OdomMsg.twist.twist.angular.y = 0;
    ROS_INFO((_robotName + " Odometry Created").c_str()); 
}

void bra::Odom::callback(const geometry_msgs::Twist::ConstPtr& msg){
    if(_enable){
        // Get data to compute
        _Time[CURRENT] = ros::Time::now();
        _velocity2Compute[CURRENT][LINEAR] = msg->linear.x;
        _velocity2Compute[CURRENT][ANGULAR] = msg->angular.z;
        // Compute
        this->compute();
        // Update and Publish the Results
        this->publish();
    } else {
        _velocity2Compute[LAST][LINEAR] = msg->linear.x;
        _velocity2Compute[LAST][ANGULAR] = msg->angular.z;
    
        _Time[LAST] = ros::Time::now();
        _enable = true;
    }
}

void bra::Odom::compute(){
    double deltaPosition[3];
    double deltaTime;
    // Compute Linear and Angular Velocity
    // Trapezoidal Rule for integration (x(t-dt) + x(t))*dt/2
    _velocity[LINEAR] = (_velocity2Compute[CURRENT][LINEAR] + _velocity2Compute[LAST][LINEAR])/2;
    _velocity[ANGULAR] = (_velocity2Compute[CURRENT][ANGULAR] + _velocity2Compute[LAST][ANGULAR])/2;
    // Compute Delta Time and Position Variation
    deltaTime = (_Time[CURRENT] - _Time[LAST]).toSec(); // Variation of Time in seconds
    deltaPosition[X] = _velocity[LINEAR] * cos(_velocity[ANGULAR]) * deltaTime;
    deltaPosition[Y] = _velocity[LINEAR] * sin(_velocity[ANGULAR]) * deltaTime;
    deltaPosition[TH] = _velocity[ANGULAR] * deltaTime;
    // Add Position Variation into Position
    _position[X] += deltaPosition[X];
    _position[Y] += deltaPosition[Y];
    _position[TH] += deltaPosition[TH];
    // Reset Variables to next call
    _velocity2Compute[LAST][LINEAR] = _velocity2Compute[CURRENT][LINEAR];
    _velocity2Compute[LAST][ANGULAR] = _velocity2Compute[CURRENT][ANGULAR];
    _Time[LAST] = _Time[CURRENT];
}

void bra::Odom::publish(){
    // Update Transform
    _OdomTransform.header.stamp = _Time[CURRENT];
    _OdomTransform.transform.translation.x = _position[X];
    _OdomTransform.transform.translation.y = _position[Y];
    _OdomTransform.transform.rotation = tf::createQuaternionMsgFromYaw(_position[TH]);
    // Update Odometry
    _OdomMsg.header.stamp = _Time[CURRENT];
    _OdomMsg.pose.pose.position.x = _position[X];
    _OdomMsg.pose.pose.position.y = _position[Y];
    _OdomMsg.pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(0,0,_position[TH]);
    _OdomMsg.twist.twist.linear.x = _velocity[LINEAR];
    _OdomMsg.twist.twist.angular.z = _velocity[ANGULAR];
    // Publish Odometry and TF
    _Broadcaster.sendTransform(_OdomTransform);
    _OdomPub.publish(_OdomMsg);
}