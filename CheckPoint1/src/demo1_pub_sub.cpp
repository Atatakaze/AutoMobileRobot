#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
using namespace std;

int flag = 0;

void callback(const std_msgs::Int32::ConstPtr &msg)
{
  //ROS_INFO("Message from Arduino is %d", msg->data);
  cout << "Message from Arduino is " << msg->data << endl;
  flag = 1;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo1_pub_sub");
  ros::NodeHandle node;

  ros::Publisher number_publisher = node.advertise<std_msgs::Int32>("/numbers", 10);
  ros::Subscriber number_subscriber = node.subscribe("/reply", 10, callback);
  ros::Rate loop_rate(10);

  int inputNumber = 0;
  std_msgs::Int32 msg;

  while(ros::ok())
  {
    cout << "User's input is ";
    cin >> inputNumber;
    msg.data = inputNumber;
    number_publisher.publish(msg);
    while(flag == 0)
    {
      ros::spinOnce();
    }
    flag = 0;
    loop_rate.sleep();
  }
  return 0;
}
