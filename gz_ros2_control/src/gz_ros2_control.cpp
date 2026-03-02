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

#include "gz_ros2_control/gz_ros2_control.hpp"

#include <condition_variable>
#include <mutex>

#include <gz/transport/Node.hh>

#include <builtin_interfaces/msg/duration.hpp>
#include <rosgraph_msgs/msg/clock.hpp>
#include <sensor_msgs/msg/joint_state.hpp>
#include <std_msgs/msg/float64.hpp>

// #include <hardware_interface/hardware_info.hpp>
// #include <hardware_interface/lexical_casts.hpp>
// #include <hardware_interface/types/hardware_interface_type_values.hpp>

#define GZ_TRANSPORT_NAMESPACE gz::transport::
#define GZ_MSGS_NAMESPACE gz::msgs::

using namespace std::literals::chrono_literals;

class gz_ros2_control::RosGzSystemPrivate
{

public:
  RosGzSystemPrivate() = default;

  ~RosGzSystemPrivate() = default;

  /// \brief Degrees of freedom.
  // size_t n_dof_;

  /// \brief last time the write method was called.
  // rclcpp::Time last_update_sim_time_ros_;

  /// \brief vector with the joint's names.
  // std::vector<struct jointData> joints_;

  /// \brief state interfaces that will be exported to the Resource Manager
  std::vector<hardware_interface::StateInterface> state_interfaces_;

  /// \brief command interfaces that will be exported to the Resource Manager
  std::vector<hardware_interface::CommandInterface> command_interfaces_;

  /// \brief controller update rate
  // unsigned int update_rate_;

  /// \brief Gazebo communication node.
  // GZ_TRANSPORT_NAMESPACE Node node_;

  // Should hold the joints if no control_mode is active
  // bool hold_joints_ = true;

  std::vector<std::string> joint_names_;
  std::vector<std::string> command_topics_;

  // Subscriptions
  rclcpp::Subscription<rosgraph_msgs::msg::Clock>::SharedPtr clock_sub_;
  rclcpp::Subscription<sensor_msgs::msg::JointState>::SharedPtr joint_sub_;

  // Publishers
  std::vector<rclcpp::Publisher<std_msgs::msg::Float64>::SharedPtr> cmd_pubs_;

  // RCL Node
  rclcpp::Node::SharedPtr node_;

  // Manage lock-step update
  rclcpp::Time last_sim_time_{0};
  rclcpp::Duration last_sim_period_{0ns};

  std::mutex mutex_;
  std::condition_variable cv_;
  bool new_step_{false};
};

namespace gz_ros2_control
{
CallbackReturn RosGzSystem::on_init(
  const hardware_interface::HardwareComponentInterfaceParams & params)
{
  if (hardware_interface::SystemInterface::on_init(params) != CallbackReturn::SUCCESS) {
    return CallbackReturn::ERROR;
  }

  for (const auto & joint : params.hardware_info.joints)
  {
    std::string name = joint.name;
    dataPtr->joint_names_.push_back(joint.name);
    dataPtr->command_topics_.push_back(joint.parameters.at("command_topic"));

    //! @todo initialise state and command interfaces
    // positions_.push_back(0.0);
    // velocities_.push_back(0.0);
    // commands_.push_back(0.0);
  }

  return CallbackReturn::SUCCESS;
}

CallbackReturn RosGzSystem::on_configure(
  const rclcpp_lifecycle::State & /*previous_state*/)
{
  RCLCPP_INFO(
    dataPtr->node_->get_logger(), "System Successfully configured!");

  return CallbackReturn::SUCCESS;
}

std::vector<hardware_interface::StateInterface>
RosGzSystem::export_state_interfaces()
{
  return std::move(dataPtr->state_interfaces_);
}

std::vector<hardware_interface::CommandInterface>
RosGzSystem::export_command_interfaces()
{
  return std::move(dataPtr->command_interfaces_);
}

CallbackReturn RosGzSystem::on_activate(
  const rclcpp_lifecycle::State & previous_state)
{
  return hardware_interface::SystemInterface::on_activate(previous_state);
}

CallbackReturn RosGzSystem::on_deactivate(
  const rclcpp_lifecycle::State & previous_state)
{
  return hardware_interface::SystemInterface::on_deactivate(previous_state);
}

hardware_interface::return_type RosGzSystem::perform_command_mode_switch(
  const std::vector<std::string> & /*start_interfaces*/,
  const std::vector<std::string> & /*stop_interfaces*/)
{
  //! @todo handle mode switch

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type RosGzSystem::read(
  const rclcpp::Time & time,
  const rclcpp::Duration & period)
{
  //! @todo handle read
  std::unique_lock<std::mutex> lock(dataPtr->mutex_);
  dataPtr->cv_.wait(lock, [this] {
    return dataPtr->new_step_;
  });
  dataPtr->new_step_ = false;

  dataPtr->last_sim_time_ = time;
  dataPtr->last_sim_period_ = period;

  return hardware_interface::return_type::OK;
}

hardware_interface::return_type RosGzSystem::write(
  const rclcpp::Time & /*time*/,
  const rclcpp::Duration & /*period*/)
{
  //! @todo handle write

  return hardware_interface::return_type::OK;
}

}  // namespace gz_ros2_control

// #include "pluginlib/class_list_macros.hpp"  // NOLINT
// PLUGINLIB_EXPORT_CLASS(
//   gz_ros2_control::RosGzSystem,
//   gz_ros2_control::hardware_interface::SystemInterface)
