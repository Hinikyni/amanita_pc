#include <ros/ros.h>
#include <odom/odom.hpp>
#include <string>

int main(int argc, char** argv){
    ros::init(argc, argv, "amanita_pc");
    ros::NodeHandle node;

    /* To use: Just instance a Odom class on "bra" namespace. Example bellow */
                        // Robot Name     // NodeHandle
    bra::Odom Odometry1("amanita_augusta", node);
    bra::Odom Odometry2("amanita_severina", node);

    ros::spin();
}