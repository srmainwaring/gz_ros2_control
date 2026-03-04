#!/usr/bin/env python3
"""Relay output from a joint_state_topic_interface controller to individual commands

Subscriptions
- sensor_msgs.msg.JointState on /robot_joint_commands

Publications
- std_msgs.msg.Float64 on /{joint_name}/cmd_pos
- std_msgs.msg.Float64 on /{joint_name}/cmd_vel

Usage

Launch Gazebo and controllers 
- ros2 launch gz_ros2_control_demos ackermann_drive_example.launch.py

Launch command relay
- python ackermann_joint_commands.py

Send command
- ros2 topic pub /ackermann_steering_controller/reference geometry_msgs/msg/TwistStamped "{twist: {linear: {x: 3.0}, angular: {z: 0.7}}}"

Inspect command
- ros2 topic echo /robot_joint_commands

"""

import math
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import JointState
from std_msgs.msg import Float64

JOINT_TOPICS = {
    "rear_left_wheel_joint": "/rear_left_wheel_joint/cmd_vel",
    "rear_right_wheel_joint": "/rear_right_wheel_joint/cmd_vel",
    "left_wheel_steering_joint": "/left_wheel_steering_joint/cmd_pos",
    "right_wheel_steering_joint": "/right_wheel_steering_joint/cmd_pos",
    "front_left_wheel_joint": "/front_left_wheel_joint/cmd_vel",
    "front_right_wheel_joint": "/front_right_wheel_joint/cmd_vel",
}


class AckermannJointCommands(Node):
    def __init__(self):
        super().__init__("ackermann_joint_commands")
        self.pubs = {}
        for joint_name, topic in JOINT_TOPICS.items():
            self.pubs[joint_name] = self.create_publisher(Float64, topic, 10)
        self.create_subscription(
            JointState, "/robot_joint_commands", self.on_joint_command, 10
        )

    def on_joint_command(self, msg):
        for i, name in enumerate(msg.name):
            if name in self.pubs:
                cmd = Float64()
                if not math.isnan(msg.position[i]):
                    # position command
                    cmd.data = msg.position[i]
                elif not math.isnan(msg.velocity[i]):
                    # velocity command
                    cmd.data = msg.velocity[i]
                self.pubs[name].publish(cmd)


def main(args=None):
    rclpy.init(args=args)
    node = AckermannJointCommands()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == "__main__":
    main()
