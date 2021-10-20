/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */

#include <ros.h>
#include "std_msgs/Int32.h"

ros::NodeHandle node;

// pwm input from raspberry pi
int pwm_r = 0;
int pwm_l = 0;

// subsciber1 callback_function
void messageGrab1(const std_msgs::Int32& msg1){
  pwm_r = msg1.data;
}
// subsciber2 callback_function
void messageGrab2(const std_msgs::Int32& msg2){
  pwm_l = msg2.data;
}

ros::Subscriber<std_msgs::Int32> s1("/pwmR", &messageGrab1);
ros::Subscriber<std_msgs::Int32> s2("/pwmL", &messageGrab2);

#define STEPSPERROUND 16
// direction = 1 -> forward
// direction = 0 -> stop
// direction = -1 -> backward
int direction_r = 0;
int direction_l = 0;

// left motor parameters
// motor direction
#define In1 10
#define In2 11
// motor encoder
#define encoderL_pinA 3
#define encoderL_pinB 9
// motor speed
#define ENA 5
int position_l = 0;

// right motor parameters
// motor direction
#define In3 12
#define In4 13
// motor encoder
#define encoderR_pinA 2
#define encoderR_pinB 8
// motor speed
#define ENB 6
int position_r = 0;
 
void setup()
{
  node.initNode();
  node.subscribe(s1);
  node.subscribe(s2);

  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(ENA, OUTPUT);
  attachInterrupt(1, IntA, CHANGE);
  
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  pinMode(ENB, OUTPUT);
  attachInterrupt(0, IntB, CHANGE);
}

void loop()
{
  
  run();
  
  node.spinOnce();
  node.spinOnce();
  
  if(pwm_l == 0)
    direction_l = 0;
  else if(pwm_l > 0)
    direction_l = 1;
  else
    direction_l = -1;
  
  if(pwm_r == 0)
    direction_r = 0;
  else if(pwm_r > 0)
    direction_r = 1;
  else
    direction_r = -1;
  
  delay(10);
}

// run
void run()
{
  if(direction_l == 0)
  {
    digitalWrite(In1, LOW);
    digitalWrite(In2, LOW);
  }
  if(direction_r == 0)
  {
    digitalWrite(In3, LOW);
    digitalWrite(In4, LOW);
  }
  if(direction_l == 1)
  {
    digitalWrite(In1, HIGH);
    digitalWrite(In2, LOW);
    analogWrite(ENA, pwm_l);
  }
  if(direction_r == 1)
  {
    digitalWrite(In3, HIGH);
    digitalWrite(In4, LOW);
    analogWrite(ENB, pwm_r);
  }
  if(direction_l == -1)
  {
    digitalWrite(In1, LOW);
    digitalWrite(In2, HIGH);
    analogWrite(ENA, -1 * pwm_l);
  }
  if(direction_r == -1)
  {
    digitalWrite(In3, LOW);
    digitalWrite(In4, HIGH);
    analogWrite(ENB, -1 * pwm_r);
  }
}

// interrupt A function
void IntA()
{
  if(position_l > STEPSPERROUND)
    position_l = 0;
  position_l++;
}

// interrupt B function
void IntB()
{
  if(position_r > STEPSPERROUND)
    position_r = 0;
  position_r++;
}
