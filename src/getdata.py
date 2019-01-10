#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

lVel = 0.0
lTime = 0
rVel = 0.0
rTime = 0
arquivo = open('/home/teo/pwmdata.txt', 'w')

def lcallback(data):
    global lVel, lTime
    lVel += (data.data)
    lTime += 1

def rcallback(data):
    global rVel, rTime
    rVel += data.data
    rTime += 1


if __name__ == '__main__':
    rospy.init_node('aquisicao', anonymous=True)
    rospy.Subscriber("amanita_cecilia_mbed/left_wheel_velocity", Float32, lcallback)
    rospy.Subscriber("amanita_cecilia_mbed/right_wheel_velocity", Float32, rcallback)
    pwmPub = rospy.Publisher('amanita_cecilia_pc/cmd_vel', Twist, queue_size=10)
    
    print("Iniciado..")
    
    pwmValue = 0.0
    msg = Twist()
    msg.linear.x = 0
    msg.angular.z = 0
    rate = rospy.Rate(0.5)
    while(pwmValue <= 1.05):
        if lTime == 0 or rTime == 0:
            continue
        print(pwmValue)
        msg.linear.x = pwmValue
        msg.angular.z = pwmValue
        pwmPub.publish(msg)
        rate.sleep()
        arquivo.write(str(pwmValue) + '    ' + str(lVel/lTime) + '    ' + str(rVel/rTime) + '\n')
        rTime = lTime = lVel = rVel = 0
        pwmValue += 0.01
    
    arquivo.close()
    print("Finalizado")
    exit(1)