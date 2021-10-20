#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo1_pub");
  ros::NodeHandle node;

  ros::Publisher number_publisher = node.advertise<std_msgs::Int32>("/numbers", 10);

  ros::Rate loop_rate(1000);

  int inputNumber;
  int count = 0;
  while(ros::ok() && count < 10)
  {
    std_msgs::Int32 msg;
    cout << "User's input is ";
    cin >> inputNumber;
    msg.data = inputNumber;

    //ROS_INFO("%d", msg.data);
    number_publisher.publish(msg);

    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}
