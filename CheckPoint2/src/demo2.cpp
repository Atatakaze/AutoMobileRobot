#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo2_rpi_node");
  ros::NodeHandle node;

  ros::Publisher number_publisher1 = node.advertise<std_msgs::Int32>("/pwmR", 10);
  ros::Publisher number_publisher2 = node.advertise<std_msgs::Int32>("/pwmL", 10);
  ros::Rate loop_rate(10);

  int pwm_r = 0;
  int pwm_l = 0;
  std_msgs::Int32 msg1;
  std_msgs::Int32 msg2;

  while(ros::ok())
  {
    cout << "User's right is ";
    cin >> pwm_r;
    cout << "User's left is ";
    cin >> pwm_l;

    msg1.data = pwm_r;
    msg2.data = pwm_l;
    number_publisher1.publish(msg1);
    number_publisher2.publish(msg2);

    loop_rate.sleep();
  }
  return 0;
}
