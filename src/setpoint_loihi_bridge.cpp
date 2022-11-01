// Copyright 2015 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <iostream>

// include ROS 1
#ifdef __clang__
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wunused-parameter"
#endif
#include "ros/ros.h"
#include "std_msgs/Float64MultiArray.h"
#ifdef __clang__
# pragma clang diagnostic pop
#endif

// include ROS 2
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "custom_interfaces/msg/user_command.hpp"


// publish ros2_px4 setpoint to loihi
ros::Publisher pub;

void setpointCallback(const custom_interfaces::msg::UserCommand::SharedPtr ros2_msg)
{
  if (ros2_msg->mode > 0) {
    std_msgs::Float64MultiArray ros1_msg;
    for (int i = 0; i < 3; i++) {
      ros1_msg.data.push_back(ros2_msg->sp[i]);
    }
    pub.publish(ros1_msg);
  }
}

int main(int argc, char * argv[])
{
  // ROS 1 node and publisher
  ros::init(argc, argv, "talker");
  ros::NodeHandle n;
  pub = n.advertise<std_msgs::Float64MultiArray>("/setpoint", 10);

  // ROS 2 node and subscriber
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("listener");
  auto sub = node->create_subscription<custom_interfaces::msg::UserCommand>(
    "user/command", rclcpp::SensorDataQoS(), setpointCallback);

  rclcpp::spin(node);

  return 0;
}
