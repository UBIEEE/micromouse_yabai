#include "Drive/Controller/incremental_controller.hpp"

#include "main.h"

using namespace drive;

void IncrementalController::reset() {
  m_linear.accel_timer.stop();
  m_linear.accel_timer.reset();
  m_linear.start_velocity_mmps = 0.f;
  m_linear.velocity_mmps       = 0.f;

  m_angular.velocity_dps = 0.f;

  m_current_motion = BasicMotion::STOP;
  m_target_pose    = {}; // TODO: Correct start pose.

  m_motion_timer.stop();
  m_motion_timer.reset();
}

Trajectory::State IncrementalController::update() {
  using enum BasicMotion;

  update_linear_velocity();

  if (TURN_LEFT == m_current_motion || TURN_RIGHT == m_current_motion) {
    update_turning();
  } else {
    m_angular.velocity_dps = 0.f;
  }

  /*

  if (reached target pose) {
    begin_next_motion();
  }

  */

  const float angular_velocity_dps =
      m_angular.right_turn ? m_angular.velocity_dps : -m_angular.velocity_dps;

  return {.linear_velocity_mmps = m_linear.velocity_mmps,
          .angular_velocity_dps = angular_velocity_dps};
}

void IncrementalController::update_linear_velocity() {
  const float& v_0 = m_linear.start_velocity_mmps;
  const float& a   = m_linear.MAX_VELOCITY_MMPS;
  const float t    = m_linear.accel_timer.elapsed_s();

  switch (m_current_motion) {
    using enum BasicMotion;
  case ROTATE_IN_PLACE:
    // We should already be stopped if this is the case.
    m_linear.velocity_mmps = 0.f;
    break;

  case STOP:
    // Decelerate.
    if (m_linear.velocity_mmps > 0.f) {
      m_linear.velocity_mmps = std::max(v_0 - a * t, 0.f);
    }
    break;

  default:
    // Every other motion should target the max velocity, so accelerate.
    if (m_linear.velocity_mmps < m_linear.MAX_VELOCITY_MMPS) {
      m_linear.velocity_mmps =
          std::min(v_0 + a * t, m_linear.MAX_VELOCITY_MMPS);
    }
    break;
  }
}

void IncrementalController::update_turning() {
  const float& a = m_angular.MAX_ACCELERATION_DPS2;
  const float& t = m_motion_timer.elapsed_s();

  const float accel_timestamp_s = m_angular.accel_time_s;
  const float const_timestamp_s = accel_timestamp_s + m_angular.const_time_s;

  if (t < accel_timestamp_s) { // Accelerate.
    m_angular.velocity_dps = std::min(m_angular.MAX_ACCELERATION_DPS2 * t,
                                      m_angular.high_velocity_dps);

  } else if (t < const_timestamp_s) { // Constant velocity.
    m_angular.velocity_dps = m_angular.high_velocity_dps;

  } else { // Decelerate.
    const float decel_time_s = t - const_timestamp_s;
    m_angular.velocity_dps =
        std::max(m_angular.high_velocity_dps - (a * decel_time_s), 0.f);
  }
}

void IncrementalController::begin_next_motion() {
  using enum BasicMotion;

  // Get the next motion.
  const BasicMotion next_motion = STOP; // MazeSearch::get().next_motion();

  // The robot needs to stop before it can rotate in place!
  if (ROTATE_IN_PLACE == next_motion) {
    assert_param(STOP == m_current_motion);
  }

  m_current_motion = next_motion;

  // Handle linear acceleration.
  m_linear.accel_timer.reset();
  m_linear.accel_timer.start();
  m_linear.start_velocity_mmps = m_linear.velocity_mmps;

  // Handle angular acceleration.
  if (TURN_LEFT == m_current_motion || TURN_RIGHT == m_current_motion) {
    // Calculate the time to target pose.
    const float total_time_s = [&] {
      const float& v_0 = m_linear.start_velocity_mmps;
      const float& v   = m_linear.MAX_VELOCITY_MMPS;
      const float& a   = m_linear.MAX_ACCELERATION_MMPS2;

      //
      // v^2 = v_0^2 + 2 * a * d
      //
      // (v^2 - v_0^2)/(2 * a) = d
      //
      const float total_accel_dist_mm = (v * v - v_0 * v_0) / (2.f * a);

      const float accel_dist_mm =
          std::min(total_accel_dist_mm, m_angular.turn.DISTANCE_MM);

      const float cruise_dist_mm = m_angular.turn.DISTANCE_MM - accel_dist_mm;

      //
      // x = v_0 * t + 1/2 * a * t^2
      //
      // t = (-v_0 + sqrt(v_0^2 + 2 * a * x)) / a
      //
      const float accel_time_s =
          (-v_0 + std::sqrt(v_0 * v_0 + 2.f * a * accel_dist_mm)) / a;

      const float cruise_time_s = cruise_dist_mm / v;

      return (accel_time_s + cruise_time_s);
    }();

    const float accel_decel_time_s = m_angular.ACCEL_TIME_S * 2;

    if (total_time_s > accel_decel_time_s) {
      m_angular.accel_time_s      = m_angular.ACCEL_TIME_S;
      m_angular.const_time_s      = total_time_s - accel_decel_time_s;
      m_angular.high_velocity_dps = m_angular.MAX_VELOCITY_DPS;
    } else {
      // Can't reach max velocity in time, so adjust the acceleration time.
      m_angular.accel_time_s = total_time_s / 2.f;
      m_angular.const_time_s = 0.f;
      m_angular.high_velocity_dps =
          m_angular.MAX_ACCELERATION_DPS2 * m_angular.accel_time_s;
    }
    m_angular.right_turn = (TURN_RIGHT == m_current_motion);

  } else if (ROTATE_IN_PLACE == m_current_motion) {
    m_angular.accel_time_s      = m_angular.rotate.ACCEL_TIME_S;
    m_angular.const_time_s      = m_angular.rotate.CONST_TIME_S;
    m_angular.high_velocity_dps = m_angular.rotate.HIGH_VELOCITY_DPS;
    m_angular.right_turn        = true;
  }

  // m_target_pose = {}; // TODO: Get the target pose from the motion.

  m_motion_timer.reset();
  m_motion_timer.start();
}
