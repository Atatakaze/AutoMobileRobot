#include "ros/ros.h"
#include "std_msgs/Int32.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
  ros::init(argc, argv, "demo2_ver2_rpi_node");
  ros::NodeHandle node;

  ros::Publisher number_publisher = node.advertise<std_msgs::Int32>("/pwm", 1);
  ros::Rate loop_rate(10);

  ros::Duration(2).sleep();
  std_msgs::Int32 msg;

  int pwmr = 0;
  int pwml = 0;
  int mode = 0;

  while(ros::ok())
  {
    cout << "User's right is ";
    cin >> pwmr;
    cout << "User's left is ";
    cin >> pwml;

    if(pwmr == 0 && pwml == 0)
      mode = 0;
    else if(pwmr > 0 && pwml > 0 && pwmr < 256 && pwml < 256)
      mode = 1;
    else if(pwmr < 0 && pwml < 0 && pwmr > -256 && pwml > -256)
    {
      mode = 2;
      pwml = -1 * pwml;
      pwmr = -1 * pwmr;
    }
    else
      break;

    msg.data = mode;
    number_publisher.publish(msg);
    ros::spinOnce();
    cout << "PUB\n";
    loop_rate.sleep();
    msg.data = pwml;
    number_publisher.publish(msg);
    ros::spinOnce();
    cout << "PUB\n";
    loop_rate.sleep();
    msg.data = pwmr;
    number_publisher.publish(msg);
    ros::spinOnce();
    cout << "PUB\n";
    loop_rate.sleep();
  }
}
