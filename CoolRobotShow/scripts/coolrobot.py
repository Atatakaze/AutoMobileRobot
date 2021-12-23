#!/usr/bin/env python

import rospy
import time
from std_msgs.msg import Int32
import RPi.GPIO as GPIO

trigger_pin = 2	# ultrasound trigger pin
echo_pin = 3	# ultrasound echo pin
dis = 0
TURN_COUNTER = 0
MAX_COUNTER = 37
TURN_FLAG = False

def send_trigger_pulse():
  GPIO.output(trigger_pin, True)
  time.sleep(0.001)
  GPIO.output(trigger_pin, False)

def wait_for_echo(value, timeout):
  count = timeout
  while GPIO.input(echo_pin) != value and count > 0:
    count = count - 1

def get_distance():
  send_trigger_pulse()
  wait_for_echo(True, 5000)
  start = time.time()
  wait_for_echo(False, 5000)
  finish = time.time()
  pulse_len = finish - start
  distance = pulse_len * 17348 - 5.8844
  return distance

if __name__ == '__main__':
  try:
    # === GPIO pins setting === #
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(trigger_pin, GPIO.OUT)
    GPIO.setup(echo_pin, GPIO.IN)

    pub = rospy.Publisher('/mode', Int32, queue_size=10)
    rospy.init_node('coolrobot_rpi_node', anonymous=True)
    rate = rospy.Rate(10)

    while not rospy.is_shutdown():

      dis = get_distance()
      print("distance = %f" % dis)

      if dis < 6:	# the target is in front of the robot within 3 cm -> stop
        TURN_FLAG = True
        print("< ====== Target is Found ====== >")

      if TURN_FLAG:
        TURN_COUNTER += 1
        if TURN_COUNTER < 10:
          pub.publish(2)
        elif TURN_COUNTER < MAX_COUNTER:
          pub.publish(4)
        else:
          TURN_FLAG = False
          TURN_COUNTER = 0
      else:
        pub.publish(1)	# no target is in front of the robot -> move foreward

      rate.sleep()

  except rospy.ROSInterruptException:
    pass
  finally:
    print("< ===== program finished ===== >")
    GPIO.cleanup()

