#pragma once

#include "Basic/constants.hpp"
#include "Drive/Controller/controller.hpp"
#include "Drive/motion.hpp"
#include "Drive/pose.hpp"
#include "Math/math.hpp"
#include "Utilities/timer.hpp"
#include "Navigation/search_navigator.hpp"

#include <cmath>
#include <numbers>
#include <queue>

namespace drive {

//
// This controller can be used to control the robot when the desired path is
// unknown. This controller allows for movements to be defined incrementally as
// the robot traverses the maze. This controller's job is to move continuously
// between movements and remain at a moderately fast speed.
//
// Used primarily during search runs.
//
class IncrementalController : public DriveController {

  struct LinearData {
    static constexpr float MAX_VELOCITY_MMPS      = 500.f;
    static constexpr float MAX_ACCELERATION_MMPS2 = 500.f;

    float start_velocity_mmps = 0.f;
    float velocity_mmps       = 0.f;

    Timer accel_timer;
  } m_linear;

  struct AngularData {
    static constexpr float MAX_VELOCITY_DPS      = 180.f;
    static constexpr float MAX_ACCELERATION_DPS2 = 180.f;

    static constexpr float ACCEL_TIME_S =
        MAX_VELOCITY_DPS / MAX_ACCELERATION_DPS2;

    // 90 degree moving turns.
    static const struct TurnData {
    private:
      static constexpr float RADIUS_MM =
          (Constants::CELL_WIDTH_MM / 2.f) +
          ((Constants::CELL_WIDTH_MM - Constants::ROBOT_LENGTH_MM) / 2.f);

      static constexpr float ANGLE_RAD = std::numbers::pi_v<float> / 2.f;

    public:
      static constexpr float DISTANCE_MM = RADIUS_MM * ANGLE_RAD;

    } turn;

    // 180 degree stopped turns.
    static const struct RotateData {
    private:
      static constexpr float ANGLE_DEG = 180.f;

      static constexpr float ACCEL_ANGLE_DEG = std::min(
          (MAX_VELOCITY_DPS * MAX_VELOCITY_DPS) / (2.f * MAX_ACCELERATION_DPS2),
          ANGLE_DEG / 2.f);

      static constexpr float CONST_ANGLE_DEG =
          ANGLE_DEG - (2.f * ACCEL_ANGLE_DEG);

    public:
      static constexpr float ACCEL_TIME_S =
          sqrt_constexpr((2.f * ACCEL_ANGLE_DEG) / MAX_ACCELERATION_DPS2);

      static constexpr float CONST_TIME_S = CONST_ANGLE_DEG / MAX_VELOCITY_DPS;

      static constexpr float TOTAL_TIME_S = (ACCEL_TIME_S * 2) + CONST_TIME_S;

      static constexpr float HIGH_VELOCITY_DPS =
          MAX_ACCELERATION_DPS2 * ACCEL_TIME_S;

    } rotate;

    bool right_turn = false;

    float velocity_dps = 0.f;

    float accel_time_s;
    float const_time_s;

    float high_velocity_dps;

  } m_angular;

  SearchNavigator& m_navigator;

  BasicMotion m_current_motion = BasicMotion::STOP;
  Timer m_motion_timer;

  Pose m_target_pose = {};

public:
  IncrementalController(SearchNavigator& navigator)
    : m_navigator(navigator) {}

  void reset() override;
  Trajectory::State update() override;

private:
  void update_linear_velocity();
  void update_turning();

  void begin_next_motion();
};

} // namespace drive
