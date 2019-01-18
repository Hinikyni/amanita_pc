#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
#include <tf/transform_broadcaster.h>
#include <string>

namespace bra{

    class Odom{
        private:
            ros::Subscriber _OdomSub; // Recieve the data from robot
            ros::Publisher  _OdomPub; // Send the data as odometry msg
            ros::NodeHandle _Node;    // Node
            tf::TransformBroadcaster _Broadcaster; // Broadcaster
            geometry_msgs::TransformStamped _OdomTransform; // Odom Transform Message
            nav_msgs::Odometry _OdomMsg;
            void publish();
            void compute();
            void callback(const geometry_msgs::Twist::ConstPtr& msg);
        public:
            Odom(std::string robotName, ros::NodeHandle Node);
        private:
            std::string _robotName;
            double _position[3]; // 2D Coordinates and Angle
            double _velocity[3]; // Vx, Vy and Angular Velocity
            double _lastAngle;
            ros::Time _Time[2];
            double _velocity2Compute[2][2]; // New Velocity and Last Velocity  
                                            // Linear and Angular Velocity
            bool _enable;
            const double _degree = 3.1415/180;
            enum {LINEAR, ANGULAR};
            enum {X, Y, TH};
            enum {CURRENT, LAST};
    };

}