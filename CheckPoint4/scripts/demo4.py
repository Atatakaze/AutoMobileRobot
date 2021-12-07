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
BACKWARDCOUNT = 7	# the time that car moves backward after collision
STOPCOUNT = 0	# 0:disable 1,2:wait until 3
SPINCOUNT = 165	# the time that car spin after collision

irPin = 15	# IR receiver pin
ir_counter = 0	# ir counter
ir_counter_0 = 0	# counter for IR signal 0
#ir_counter_1 = 0	# counter for IR signal 1
IR_state = 0	# 0:receiving IR target->stop, 1:IR target found->go, 2-42:searching IR target->spin
IR_FLAG = False	# true: start to search IR signal
IRCOUNTER = 250	# max value of ir counter

if __name__ == '__main__':
  try:
    # === GPIO pins setting === #
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(lightPin, GPIO.IN)
    GPIO.setup(touchPin_R, GPIO.IN)
    GPIO.setup(touchPin_L, GPIO.IN)
    GPIO.setup(touchPin_B, GPIO.IN)
    GPIO.setup(irPin, GPIO.IN)

    pub = rospy.Publisher('/mode', Int32, queue_size=10)
    rospy.init_node('demo4_rpi_node', anonymous=True)
    rate = rospy.Rate(10)
    rate2 = rospy.Rate(100)

    while not rospy.is_shutdown():

      light_rev = GPIO.input(lightPin)	# light_rev = 0:detected, 1:not detected
      touch_R = GPIO.input(touchPin_R)	# touch_R = 0:pressed, 1:not pressed
      touch_L = GPIO.input(touchPin_L)	# touch_L = 0:pressed, 1:not pressed
      touch_B = GPIO.input(touchPin_B)	# touch_B = 0:pressed, 1:not pressed


      # === light bulb detect (GPIO input data) === #
      if touch_B == 0:
        print("< ===== light bulb is touched !!! ===== >")
        IR_FLAG = True

      # === IR signal searching === #
      if IR_FLAG:

        # === receiving and calculating IR signal === #
        if IR_state == 0:
          pub.publish(0)
          if ir_counter < IRCOUNTER:

            print("receiving...")
            ir_rev = GPIO.input(irPin)
            #print(ir_rev)

            if ir_rev == 0:
              ir_counter_0 += 1
            #if ir_rev == 1:
              #ir_counter_1 += 1

            ir_counter += 1

          else:
            print("ir_counter_0 = ", ir_counter_0)
            #print("ir_counter_1 = ", ir_counter_1)

            if ir_counter_0 > 65 and ir_counter_0 < 90:
              IR_state = 1
	    elif ir_counter_0 < 60 and ir_counter_0 > 40:
	      IR_state = 1
            else:
              IR_state = 2

            ir_counter = 0
            ir_counter_0 = 0
            #ir_counter_1 = 0

        # === IR signal is found, go straight === #
        elif IR_state == 1:
          print("IR target is found")
          pub.publish(1)
          break

        # === searching IR signal, rotating === #
        else:
          print("searching...")
          if IR_state == 42:
            pub.publish(0)
            IR_state = 0
          else:
            pub.publish(3)
            IR_state += 1

        rate2.sleep()



      else:

        # === target detect (GPIO input data) === #
        target_last = target
        if(light_rev == 0):
          target = True
          print("light")
        else:
          target = False
        #print("light_rev = :", light_rev)

        # === collision detect (GPIO input data) === #
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
            #if target == 0:
            #  pub.publish(1)
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
              counter_L = BACKWARDCOUNT
              pub.publish(4)

        rate.sleep()

  except rospy.ROSInterruptException:
    pass
  finally:
    print("< ===== program finished ===== >")
    GPIO.cleanup()

