// Copyright 2026 Rhys Mainwaring.
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

#ifndef GZ_ROS2_CONTROL__GZ_ROS2_CONTROL_HPP_
#define GZ_ROS2_CONTROL__GZ_ROS2_CONTROL_HPP_

#include <memory>
#include <string>
#include <vector>

#include <hardware_interface/system_interface.hpp>

#include <rclcpp/rclcpp.hpp>

namespace gz_ros2_control
{

using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

// Forward declaration
class RosGzSystemPrivate;

class RosGzSystem : public hardware_interface::SystemInterface
{
public:
  // Documentation Inherited
  CallbackReturn on_init(
    const hardware_interface::HardwareComponentInterfaceParams & params) override;

  CallbackReturn on_configure(
    const rclcpp_lifecycle::State & previous_state) override;

  // Documentation Inherited
  std::vector<hardware_interface::StateInterface>
  export_state_interfaces() override;

  // Documentation Inherited
  std::vector<hardware_interface::CommandInterface>
  export_command_interfaces() override;

  // Documentation Inherited
  CallbackReturn on_activate(
    const rclcpp_lifecycle::State & previous_state) override;

  // Documentation Inherited
  CallbackReturn on_deactivate(
    const rclcpp_lifecycle::State & previous_state) override;

  // Documentation Inherited
  hardware_interface::return_type perform_command_mode_switch(
    const std::vector<std::string> & start_interfaces,
    const std::vector<std::string> & stop_interfaces) override;

  // Documentation Inherited
  hardware_interface::return_type read(
    const rclcpp::Time & time,
    const rclcpp::Duration & period) override;

  // Documentation Inherited
  hardware_interface::return_type write(
    const rclcpp::Time & time,
    const rclcpp::Duration & period) override;


  /// \brief Initialize the system interface
  bool init_sim(
    rclcpp::Node::SharedPtr & model_nh,
    const hardware_interface::HardwareInfo & hardware_info,
    unsigned int update_rate);

private:
  /// \brief Private data class
  std::unique_ptr<RosGzSystemPrivate> dataPtr;

  // private:
//   std::shared_ptr<rclcpp::Node> node_;

//   rclcpp::Subscription<rosgraph_msgs::msg::Clock>::SharedPtr clock_sub_;
//   rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_sub_;

//   std::vector<rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr> cmd_pubs_;

//   std::vector<std::string> joint_names_;
//   std::vector<std::string> command_topics_;

//   std::vector<double> positions_;
//   std::vector<double> velocities_;
//   std::vector<double> commands_;

//   std::mutex mutex_;
//   std::condition_variable cv_;
//   bool new_step_{false};

//   rclcpp::Time last_sim_time_;
//   rclcpp::Duration sim_period_;
//   std::thread executor_thread_;
};

}  // namespace gz_ros2_control

#endif  // GZ_ROS2_CONTROL__GZ_ROS2_CONTROL_HPP_
