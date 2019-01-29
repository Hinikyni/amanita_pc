#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <waypoint/waypoint.hpp>

int main(int argc, char** argv){
    ros::init(argc, argv, "amanita_waypoint");
    ros::Duration t(1);
    bra::Waypoint amanita_waypoint("amanita_cecilia");

    // Init Position
    amanita_waypoint.run(0, 0, 0);
    t.sleep();
    amanita_waypoint.run(1.00, 0, 0);
    /*// 90 Degrees
    amanita_waypoint.run(0, 0, 1.57079632679);
    t.sleep();
    // 180 Degress
    amanita_waypoint.run(0,0, 3.14159265359);
    t.sleep();
    // 270 Degrees
    amanita_waypoint.run(0,0, -1.57079632679);
    t.sleep();
    // 360 Degrees
    amanita_waypoint.run(0, 0, 0);
    t.sleep();*/
}