/* 
 * rosserial::std_msgs::Float64 Test
 * Receives a Float64 input, subtracts 1.0, and publishes it
 */

#include <ros.h>
#include <std_msgs/Int32.h>


ros::NodeHandle node;

float x; 
int flag = 0;

void messageGrab( const std_msgs::Int32& msg){
  x = msg.data * 2;
  flag = 0;
}

std_msgs::Int32 value;
ros::Subscriber<std_msgs::Int32> s("/numbers", &messageGrab);
ros::Publisher p("/reply", &value);

void setup()
{
  node.initNode();
  node.advertise(p);
  node.subscribe(s);
}

void loop()
{
  value.data = x;
  if(flag == 0)
  {
    p.publish( &value );
    flag = 1;
  }
  node.spinOnce();
  delay(10);
}

