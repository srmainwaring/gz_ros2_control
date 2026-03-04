#!/usr/bin/env python3
"""Forwards arm_controller state from join trajectory action server to joint gz pos commands."""

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
