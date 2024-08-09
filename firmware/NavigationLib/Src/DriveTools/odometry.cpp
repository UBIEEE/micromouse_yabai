#include "DriveTools/odometry.hpp"

#include "Basic/constants.hpp"
#include <cmath>

using namespace drive;

void Odometry::update(Pose& pose, const float& delta_left,
                      const float& delta_right) {

  const float& b = Constants::RobotDimensions::TRACK_WIDTH_MM;

  const float dist        = (delta_right + delta_left) / 2.f;
  const float delta_theta = (delta_right - delta_left) / b;

  const float delta_x = dist * std::cos(pose.theta_rad + (delta_theta / 2.f));
  const float delta_y = dist * std::sin(pose.theta_rad + (delta_theta / 2.f));

  pose.x_mm      += delta_x;
  pose.y_mm      += delta_y;
  pose.theta_rad += delta_theta;
}
