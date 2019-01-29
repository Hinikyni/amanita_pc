#!/usr/bin/env python
# Odom msgs
import rospy
from tf.transformations import euler_from_quaternion, quaternion_from_euler
from nav_msgs.msg import Odometry
from geometry_msgs.msg import Twist

position = [0.00, 0.00, 0.00]
velocity = [0.00, 0.00]
commands = [0.00, 0.00]

def odomCallback(data):
    global position, velocity
    # Getting Positions {x, y, theta}
    position[0] = data.pose.pose.position.x
    position[1] = data.pose.pose.position.y
    orientation_q = data.pose.pose.orientation
    orientation_list = [orientation_q.x, orientation_q.y, orientation_q.z, orientation_q.w]
    (_,_,position[2]) = euler_from_quaternion(orientation_list)
    # Getting Linear and Angular Velocity
    velocity[0] = data.twist.twist.linear.x
    velocity[1] = data.twist.twist.angular.z

def cmdCallback(data):
    global commands
    commands[0] = data.linear.x
    commands[1] = data.angular.z

if __name__ == '__main__':
    rospy.init_node('aquisicao', anonymous=True)
    # Odom Subscriber
    rospy.Subscriber("/amanita_cecilia_pc/odom", Odometry, odomCallback)
    # Command Velocity Subscriber
    rospy.Subscriber("/amanita_cecilia_pc/cmd_vel", Twist, cmdCallback)
    # Open File
    file = open('/home/teo/Documentos/CIMATEC/Iniciacao/Amanita_Producao/article/data_3.txt', 'w')
    file.write("time" + '\t' + "x" + '\t' + "y" + '\t' + "theta" + '\t' + "vx_input" + '\t' + "vz_input" + '\t' + "vx" + '\t' + "vz" + '\n')
    # Rate
    rate = rospy.Rate(100)
    while(not rospy.is_shutdown()):
        currentTime = rospy.Time.now().to_sec() # Get Current Time in Seconds
        file.write(str(currentTime) + '\t' + str(position[0]) + '\t' + str(position[1]) + '\t' + str(position[2]) + '\t' + str(commands[0]) + '\t' + str(commands[1]) + '\t' + str(velocity[0]) + '\t' + str(velocity[1]) + '\n')
        rate.sleep()
    
    file.close()
    exit(1)


