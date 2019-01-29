
#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <string>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

namespace bra{

    class Waypoint {
        
        public:
        Waypoint(std::string);
        bool run(float, float, float);
        //void addPoint();
        private:
        MoveBaseClient* _client;
        std::string _robotName;


    };

}