#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>
using namespace std;

void number_callback(const std_msgs::Int32::ConstPtr& msg)
{
  //ROS_INFO("Message from Arduino is %d", msg->data);
  cout << "Message from Arduino is " << msg->data << endl;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo1_sub");
  ros::NodeHandle node_obj;
  ros::Subscriber number_subscriber = node_obj.subscribe("/reply", 10, number_callback);
  while(ros::ok())
  {
    ros::spinOnce();
  }
  return 0;
}
