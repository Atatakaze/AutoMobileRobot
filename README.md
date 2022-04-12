# **AutoMobileRobot** #

## **Program** ##

> AutoMobileRobot @ NYCU 
>
> Using ROS to communicate between Raspberry pi and Arduino and control auto mobile car.

## **Settings** ##

- **Pins connected to raspberry pi**
  
      lightPin: BCM 14	-> connect to light sensor 
      touchPin_R: BCM 4	-> connect to touch sensor (right)
      touchPin_L: BCM 3	-> connect to touch sensor (left)
      touchPin_B: BCM = 2	-> connect to touch sensor (bottom)
      irPin: BCM = 15	        -> connect to IR sensor
  
- **Pins connected to arduino**

      encoder1pinA: pin 3     -> the interrupt pin 1, connect to encoder1 pinA
      encoder1pinB: pin 9     -> connect to encoder1 pinB
      in1: pin 10             -> connect to L298N motor driver board
      in2: pin 11             -> connect to L298N motor driver board
      ena: pin 5              -> PWM output of the left motor, coneect to L298N driver
      encoder0pinA: pin 2     -> the interrupt pin 0, connect to encoder0 pinA
      encoder0pinB: pin 8     -> connect to encoder0 pinB
      in3: pin 12             -> connect to L298N motor driver board
      in4: pin 13             -> connect to L298N motor driver board
      enb: pin 6              -> PWM output of the right motor, coneect to L298N driver

## **Instruction** ##

CheckPoint folders are the src, scripts, launch and CMakeLists of the ros package in each check point.
  
[sketchbook folder](https://github.com/Atatakaze/AutoMobileRobot/tree/main/sketchbook) is the code for arduino.
  


- ### CheckPoint1 ###
  
  Run `roslaunch ros_pub_sub demo1.launch` to launch roscore, arduino_serial_node and demo1_node. 
  
  #### Goal ####
  
  1. Input a number in keyboard and publish it in a topic.
  2. Arduino subscribe to the topic and multiple input number with 2.
  3. Arduino publish a new topic with calculation result in it.
  4. Raspberry pi subscribe to the topic and show it on the screen.
  
- ### CheckPoint2 ###

  Run `roslaunch demo2_pkg demo2.launch` to launch roscore, arduino_serial_node and demo2_node.
  
  #### Goal ####

  1. Raspberry pi collects the desired velocity of motors from the keyboard and publish it in a topic.
  2. Arduino subscribes to the topic and controls motors with PID controller.
  
- ### CheckPoint3 ###

  Run `roslaunch demo3_pkg demo3.launch` to launch roscore, arduino_serial_node and rpi_node.
  
  #### Goal ####
  
  1. Raspberry pi collects sensors' data, including 3 touch sensors and 1 light sensor, and publish different mode in a topic to achieve obstacle avoidance and target, LED light bulb, searching.
  2. Arduino subscribe to the topic and controls motors with PID controller.
  
- ### CheckPoint4 ###

  Run `roslaunch demo4_pkg demo4.launch` to launch roscore, arduino_serial_node and rpi_node.
  
  #### Goal ####
   
  1. Raspberry pi collects sensors' data, including 3 touch snesors, 1 light sensor and 1 IR sensor, and publish different mode in a topic to achieve obstacle avoidance and target searching.
  2. Different from CheckPoint3, robot has to bring the target to specific place by using IR sensor after it collects the target.
  3. Arduino subscribe to the topic and controls motors with PID controller.

- ### RobotContest ###

  Run `roslaunch RobotContest Contest.launch` to launch roscore, arduino_serial_node and rpi_node.
  
  #### Goal ####

  1. Raspberry pi collects sensors' data, including 3 touch snesors, 1 light sensor and 1 IR sensor, and publish different mode in a topic to achieve obstacle avoidance and target searching.
  2. Different from CheckPoint3, robot has to bring the target to specific place by using IR sensor after it collects the target.
  3. Arduino subscribe to the topic and controls motors with PID controller.

- ### CoolRobotShow ###

  Run `roslaunch CoolRobotShow final.launch` to launch roscore, arduino_serial_node and 2 rpi_node.

  #### Goal ####

  1. The first node in the Raspberry pi continuously collects and publishes surrounding information by using RPi Lidar.
  2. The second node in the Raspberry pi determines which way is clear, front, right and left, by subscribing to the topic that published by the first node. And then, it controls the robot to move toward the direction that is clear. 
  3. Arduino subscribe to the topic and controls motors with PID controller

## **Authors**

- **bobo** - *Initial work* -

## **License** ##

This project is licensed under MIT License - see the [LICENSE.md](https://github.com/Atatakaze/AutoMobileRobot/blob/main/LICENSE) file for details.
