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

#ifndef GZ_ROS2_CONTROL__GZ_ROS2_LOCKSTEP_PLUGIN_HPP_
#define GZ_ROS2_CONTROL__GZ_ROS2_LOCKSTEP_PLUGIN_HPP_

#include <memory>

#include <gz/sim/System.hh>
namespace sim = gz::sim;

namespace gz_ros2_control
{
// Forward declarations.
class GazeboSimROS2LockstepPluginPrivate;

class GazeboSimROS2LockstepPlugin
  : public sim::System,
  public sim::ISystemPostUpdate
{
public:
  /// \brief Constructor
  GazeboSimROS2LockstepPlugin();

  /// \brief Destructor
  ~GazeboSimROS2LockstepPlugin() override;

  void PostUpdate(
    const sim::UpdateInfo & _info,
    const sim::EntityComponentManager & _ecm) override;

private:
  /// \brief Private data pointer.
  std::unique_ptr<GazeboSimROS2LockstepPluginPrivate> dataPtr;
};
}  // namespace gz_ros2_control

#endif  // GZ_ROS2_CONTROL__GZ_ROS2_LOCKSTEP_PLUGIN_HPP_
