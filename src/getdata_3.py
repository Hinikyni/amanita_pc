#!/usr/bin/env python
import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32

lVel = 0.0
lTime = 0
rVel = 0.0
rTime = 0
WHEELS_RADIUS = 0.0325
reference_left = 0.0
reference_right = 0.0
arquivo = open('/home/teo/Documentos/CIMATEC/Iniciacao/Amanita_Producao/speed_measure_navstack/speed_measure_navstack_6.txt', 'w')
count = 0
completed = False

def lcallback(data):
    global lVel, lTime
    lVel += (data.data)
    lTime += 1

def rcallback(data):
    global rVel, rTime
    rVel += data.data
    rTime += 1

def cmdvelcallback(data):
    global reference_left, reference_right, WHEELS_RADIUS, count
    count += 1
    reference_left =  (data.linear.x - data.angular.z * WHEELS_RADIUS * 0.5)
    reference_right = (data.linear.x + data.angular.z * WHEELS_RADIUS * 0.5)

def stopCallback(data):
    arquivo.close()
    print("Programa Finalizado")
    completed = True

if __name__ == '__main__':
    rospy.init_node('aquisicao', anonymous=True)
    rospy.Subscriber("amanita_cecilia_mbed/left_wheel_velocity", Float32, lcallback)
    rospy.Subscriber("amanita_cecilia_mbed/right_wheel_velocity", Float32, rcallback)
    rospy.Subscriber("amanita_cecilia_pc/cmd_vel", Twist, cmdvelcallback)    
    rospy.Subscriber("stop", Float32, stopCallback)
    arquivo.write( "time" + '\t' + "input_l" + '\t' + "input_r" + '\t' + "l" + '\t' + "r" + '\n')

    while(count == 0):
        continue

    print("Iniciado..")
    
    while(not completed):
        if lTime == 0 or rTime == 0:
            continue

        currentTime = rospy.Time.now().to_sec()
        arquivo.write( str(currentTime) + '\t' + str(reference_left) + '\t' + str(reference_right) + '\t' + str(lVel/lTime) + '\t' + str(rVel/rTime) + '\n')
        rTime = lTime = lVel = rVel = 0

    arquivo.close()
    exit(1)