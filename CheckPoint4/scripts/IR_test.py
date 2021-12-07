#!/usr/bin/env python

import rospy
import time
from std_msgs.msg import Int32
import RPi.GPIO as GPIO

irPin = 15
ir_rev_last = 1
ir_counter_0 = 0
ir_counter_1 = 0
ir_counter = 0

if __name__ == '__main__':
  try:
    # === GPIO pins setting === #
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(irPin, GPIO.IN)

    pub = rospy.Publisher('/value', Int32, queue_size=10)
    rospy.init_node('demo4_rpi_node', anonymous=True)
    rate = rospy.Rate(100)

    while not rospy.is_shutdown():

      if ir_counter < 500:

        ir_rev = GPIO.input(irPin)

        if ir_rev == 0:
          ir_counter_0 += 1
          #print("0++")
        if ir_rev == 1:
          ir_counter_1 += 1
          #print("1++")
        '''
        if ir_rev != ir_rev_last:
          #print("changed")
          if ir_rev == 0:
            #print("0 rstart")
            #print("ir_counter_0 = ", ir_counter_0)
            #print("ir_counter_1 = ", ir_counter_1)
            #print("ratio = ", ir_counter_0 / (ir_counter_1 + ir_counter_0))
            ir_counter_0 = 0
          if ir_rev == 1:
            #print("1 restart")
            ir_counter_1 = 0

        ir_rev_last = ir_rev
        '''
        ir_counter += 1
        #print(ir_rev)
        rate.sleep()
      else:
        print("ir_counter_0 = ", ir_counter_0)
        print("ir_ocunter_1 = ", ir_counter_1)
        ir_counter = 0
        ir_counter_1 = 0
        ir_counter_0 = 0
  except rospy.ROSInterruptException:
    pass
  finally:
    print("< ===== program finished ===== >")
    GPIO.cleanup()

