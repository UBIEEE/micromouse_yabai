#include "Basic/constants.hpp"
#include "DriveTools/odometry.hpp"
#include <gtest/gtest.h>
#include <numbers>

using namespace drive;

TEST(Odometry, TestUpdate) {
  const Pose START_POSE {.x_mm = 0.f, .y_mm = 0.f, .theta_rad = 0.f};

  // Forward movement.

  Pose pose = START_POSE;
  Odometry::update(pose, 100.f, 100.f);

  ASSERT_FLOAT_EQ(100.f, pose.x_mm);
  ASSERT_FLOAT_EQ(0.f, pose.y_mm);
  ASSERT_FLOAT_EQ(0.f, pose.theta_rad);

  const float ANGLE_90_DEG = std::numbers::pi_v<float> / 2.f;
  // s = θr
  const float DIST_90_DEG = ANGLE_90_DEG * Constants::ROBOT_TRACK_WIDTH_MM;

  // Left 90 degree turn.

  pose = START_POSE;
  Odometry::update(pose, 0, DIST_90_DEG);

  ASSERT_FLOAT_EQ(ANGLE_90_DEG, pose.theta_rad);

  // Right 90 degree turn.

  pose = START_POSE;
  Odometry::update(pose, DIST_90_DEG, 0);

  ASSERT_FLOAT_EQ(-ANGLE_90_DEG, pose.theta_rad);

  // Left 180 degree turn (in place).

  pose = START_POSE;
  Odometry::update(pose, -DIST_90_DEG, DIST_90_DEG);

  ASSERT_FLOAT_EQ(0.f, pose.x_mm);
  ASSERT_FLOAT_EQ(0.f, pose.y_mm);
  ASSERT_FLOAT_EQ(ANGLE_90_DEG * 2.f, pose.theta_rad);

  const float ANGLE_180_DEG = std::numbers::pi_v<float>;
  // s = θr
  const float DIST_180_DEG = ANGLE_180_DEG * Constants::ROBOT_TRACK_WIDTH_MM;

  // Left 360 degree turn (in place).

  pose = START_POSE;
  Odometry::update(pose, -DIST_180_DEG, DIST_180_DEG);

  ASSERT_FLOAT_EQ(0.f, pose.x_mm);
  ASSERT_FLOAT_EQ(0.f, pose.y_mm);
  ASSERT_FLOAT_EQ(ANGLE_180_DEG * 2, pose.theta_rad);

  // Right 360 degree turn (in place).

  Odometry::update(pose, DIST_180_DEG, -DIST_180_DEG);

  ASSERT_FLOAT_EQ(0.f, pose.x_mm);
  ASSERT_FLOAT_EQ(0.f, pose.y_mm);
  ASSERT_FLOAT_EQ(0.f, pose.theta_rad);
}
