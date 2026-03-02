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

#include "gz_ros2_control/gz_ros2_lockstep_plugin.hpp"

#include <chrono>

#include <gz/plugin/Register.hh>
#include <gz/sim/Conversions.hh>
#include <gz/sim/System.hh>
#include <gz/transport/Node.hh>
#include <gz/msgs/clock.pb.h>
#include <gz/msgs/time.pb.h>

namespace gz_ros2_control
{

//////////////////////////////////////////////////
class GazeboSimROS2LockstepPluginPrivate
{
  /// \brief The communication node
  public: gz::transport::Node node;

  /// \brief The publisher
  public: std::unique_ptr<gz::transport::Node::Publisher> clockPub;
};

//////////////////////////////////////////////////
GazeboSimROS2LockstepPlugin::GazeboSimROS2LockstepPlugin()
  : dataPtr(std::make_unique<GazeboSimROS2LockstepPluginPrivate>()) 
{
}

//////////////////////////////////////////////////
GazeboSimROS2LockstepPlugin::~GazeboSimROS2LockstepPlugin()
{
}

//////////////////////////////////////////////////
void GazeboSimROS2LockstepPlugin::PostUpdate(
  const sim::UpdateInfo & _info,
  const sim::EntityComponentManager & /*_ecm*/)
{
  if (!this->dataPtr->clockPub)
  {
    this->dataPtr->clockPub = std::make_unique<gz::transport::Node::Publisher>(
        this->dataPtr->node.Advertise<gz::msgs::Clock>("/post_update/clock"));
  }

  gz::msgs::Clock msg;

  msg.mutable_sim()->CopyFrom(
      gz::sim::convert<gz::msgs::Time>(_info.simTime));
  this->dataPtr->clockPub->Publish(msg);
}
}  // namespace gz_ros2_control

GZ_ADD_PLUGIN(gz_ros2_control::GazeboSimROS2LockstepPlugin,
              gz::sim::System,
              gz_ros2_control::GazeboSimROS2LockstepPlugin::ISystemPostUpdate)
