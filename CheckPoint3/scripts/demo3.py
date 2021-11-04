#!/usr/bin/env python

import rospy
import time
from std_msgs.msg import Int32
import RPi.GPIO as GPIO

lightPin = 14	# light sensor
touchPin_R = 4	# touch sensor (right)
touchPin_L = 3	# touch sensor (left)
touchPin_B = 2	# touch sensor (bottom)

counter_R = 0	# 0:clear 1,2,3...:pressed counter
counter_L = 0	# 0:clear 1,2,3...:pressed counter
target = False	# false:no target detected, true:target detected
target_last = False	# last status of target
BACKWARDCOUNT = 5	# the time that car moves backward after collision
STOPCOUNT = 0	# 0:disable 1,2:wait until 3
SPINCOUNT = 165	# the time that car spin after collision

if __name__ == '__main__':
  try:
    # === GPIO pins setting === #
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(lightPin, GPIO.IN)
    GPIO.setup(touchPin_R, GPIO.IN)
    GPIO.setup(touchPin_L, GPIO.IN)
    GPIO.setup(touchPin_B, GPIO.IN)

    pub = rospy.Publisher('/mode', Int32, queue_size=10)
    rospy.init_node('demo3_rpi_node', anonymous=True)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():

     light_rev = GPIO.input(lightPin)	# light_rev = 0:detected, 1:not detected
     touch_R = GPIO.input(touchPin_R)	# touch_R = 0:pressed, 1:not pressed
     touch_L = GPIO.input(touchPin_L)	# touch_L = 0:pressed, 1:not pressed
     touch_B = GPIO.input(touchPin_B)	# touch_B = 0:pressed, 1:not pressed

     # === target detect (GPIO input data) === #
     target_last = target
     if(light_rev == 0):
       target = True
     else:
       target = False
     #print("light_rev = :", light_rev)

     # === collision detect (GPIO input data) === #
     if touch_B == 0:
       print("< ===== light bulb is touched !!! ===== >")
       pub.publish(0)
       break
     if touch_R == 0:
       print("< ===== right side collides !!! ===== >")
       counter_R = 1
     if touch_L == 0:
       print("< ===== left side collides !!! ===== >")
       counter_L = 1

     # === collision coccurs === #
     # right side collides
     if counter_R != 0:
       if counter_R < BACKWARDCOUNT:	# move backward
         pub.publish(2)
         print("move backward")
         counter_R += 1
       elif counter_R < SPINCOUNT:	# turn left
         pub.publish(3)
         print("turn left")
         counter_R += 1
       else:
         print("counter_R zero")
         counter_R = 0;
     # left side collides
     if counter_L != 0:
       if counter_L < BACKWARDCOUNT:	# move backward
         pub.publish(2)
         print("move backward")
         counter_L += 1
       elif counter_L < SPINCOUNT:	# turn right
         pub.publish(4)
         print("turn right")
         counter_L += 1
       else:
         print("counter_L zero")
         counter_L = 0;

     # === target status changed ? === #
     if target ^ target_last:	# target status change => stop
       STOPCOUNT = 1
       counter_R = 0
       counter_L = 0
       pub.publish(0)
       if target:
         print("< ===== target detected !!! ===== >")
       else:
         print("< ===== target lost !!! ===== >")

     else:	# target status doesn't change
       if STOPCOUNT == 0:	# normal
       	 if counter_R == 0 and counter_L == 0:	# no collision => move forward, collision keep rotating
           pub.publish(1)
       elif STOPCOUNT > 0 and STOPCOUNT < 3:	# stay still to search or pause until STOPCOUNT = 3 
         pub.publish(0)
         STOPCOUNT += 1
       else:
         STOPCOUNT = 0
         if target:	# target detected => forward
           pub.publish(1)
         else:	# target lost => search
           counter_R = BACKWARDCOUNT
           pub.publish(3)





     rate.sleep()

  except rospy.ROSInterruptException:
    pass
  finally:
    print("< ===== program finished ===== >")
    GPIO.cleanup()

