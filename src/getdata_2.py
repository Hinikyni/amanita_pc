#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

lVel = 0.0
lTime = 0
rVel = 0.0
rTime = 0
reference = 0.0
arquivo = open('/home/teo/velocity.txt', 'w')
twice = 0

def lcallback(data):
    global lVel, lTime
    lVel += (data.data)
    lTime += 1

def rcallback(data):
    global rVel, rTime
    rVel += data.data
    rTime += 1

def cmdvelcallback(data):
    global reference, twice
    twice += 1
    reference = data.linear.x


if __name__ == '__main__':
    rospy.init_node('aquisicao', anonymous=True)
    rospy.Subscriber("amanita_cecilia_mbed/left_wheel_velocity", Float32, lcallback)
    rospy.Subscriber("amanita_cecilia_mbed/right_wheel_velocity", Float32, rcallback)
    rospy.Subscriber("amanita_cecilia_pc/cmd_vel", Twist, cmdvelcallback)    
        
    while(twice == 0):
        continue

    print("Iniciado..")


    while(twice != 3):
        if lTime == 0 or rTime == 0:
            continue

        currentTime = rospy.Time.now().to_sec()
        arquivo.write( str(currentTime) + '\t' + str(reference) + '\t' + str(lVel/lTime) + '\t' + str(rVel/rTime) + '\n')
        rTime = lTime = lVel = rVel = 0
    
    arquivo.close()
    print("Finalizado")
    exit(1)