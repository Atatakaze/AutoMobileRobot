# **AutoMobileRobot** #

***

## **Program:** ##

> Using ROS to communicate between Raspberry pi and Arduino and control auto mobile car.

## **Instruction** ##

CheckPoint folders are the src, scripts, launch and CMakeLists of the ros package in each check point.
  
sketchbook folder is the code for arduino.
  
*_< ==== pins connected to raspberry pi ==== >_*
  
    *_lightPin: BCM 14	-> connect to light sensor_*
  
    *_touchPin_R: BCM 4	-> connect to touch sensor (right)_*
  
    *_touchPin_L: BCM 3	-> connect to touch sensor (left)_*
  
    *_touchPin_B: BCM = 2	-> connect to touch sensor (bottom)_*
  
*_< ==== pins connected to arduino ==== >_*

    *_encoder1pinA: pin 3 -> the interrupt pin 1, connect to encoder1 pinA_*

    *_encoder1pinB: pin 9 -> connect to encoder1 pinB_*

    *_in1: pin 10 -> connect to L298N motor driver board_*

    *_in2: pin 11 -> connect to L298N motor driver board_*

    *_ena: pin 5 -> PWM output of the left motor, coneect to L298N driver

    *_encoder0pinA: pin 2 -> the interrupt pin 0, connect to encoder0 pinA_*

    *_encoder0pinB: pin 8 -> connect to encoder0 pinB_*

    *_in3: pin 12 -> connect to L298N motor driver board_*

    *_in4: pin 13 -> connect to L298N motor driver board_*

    *_enb: pin 6 -> PWM output of the right motor, coneect to L298N driver

- ## CheckPoint1 ##
  
  1. Input a number in keyboard and publish it in a topic
  2. Arduino subscribe to the topic and multiple input number with 2
  3. Arduino publish a new topic with calculation result in it
  4. Raspberry pi subscribe to the topic and show it on the screen
  
  Run `roslaunch ros_pub_sub demo1.launch` to launch roscore, arduino_serial_node and demo1_node. 
  
- ## CheckPoint2 ##

  1. Raspberry pi collects the desired velocity of motors from the keyboard and publish it in a topic
  2. Arduino subscribes to the topic and controls motors with PID controller
  
  Run `roslaunch demo2_pkg demo2.launch` to launch roscore, arduino_serial_node and demo2_node.
  
- ## CheckPoint3 ##

  1. Raspberry pi collects sensors' data and publish different mode in a topic to achieve obstacle avoidance and target searching
  2. Arduino subscribe to the topic and controls motors with PID controller

  Run `roslaunch demo3_pkg demo3.launch` to launch roscore, arduino_serial_node and rpi_node.
