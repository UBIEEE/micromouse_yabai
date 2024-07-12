#include "Drive/trajectory.hpp"

#include "main.h"

#include <algorithm>
#include <cmath>
#include <numbers>

Trajectory::Trajectory(EndBehavior end_behavior,
                       const SpeedLimits& speed_limits)
  : m_speed_limits(speed_limits),
    m_end_behavior(end_behavior) {}

Trajectory Trajectory::create_linear(EndBehavior end_behavior,
                                     float distance_mm,
                                     const SpeedLimits& speed_limits) {
  Trajectory t(end_behavior, speed_limits);
  t.construct_linear_trajectory(distance_mm);
  return t;
}

Trajectory Trajectory::create_arc(EndBehavior end_behavior, float radius_mm,
                                  float angle_deg,
                                  const SpeedLimits& speed_limits) {
  assert_param(angle_deg > 0.f);

  Trajectory t(end_behavior, speed_limits);
  t.construct_arc_trajectory(radius_mm, angle_deg);
  return t;
}

Trajectory::State Trajectory::sample(float time_s) {
  assert_param(time_s >= 0.f);

  State state;
  state.angular_velocity_dps = m_speed_limits.max_angular_velocity_dps;

  if (time_s < m_time_rise_s) {
    // Accelerate
    const float v = m_end_behavior.start_velocity_mmps +
                    m_speed_limits.max_acceleration_mmps2 * time_s;

    state.linear_velocity_mmps = v;

  } else if (time_s < m_time_rise_s + m_time_cruise_s) {
    // Cruise
    state.linear_velocity_mmps = m_max_velocity_mmps;

  } else if (time_s < m_time_total_s) {
    // Decelerate
    const float t = time_s - (m_time_rise_s + m_time_cruise_s);
    const float v =
        m_max_velocity_mmps - m_speed_limits.max_acceleration_mmps2 * t;

    state.linear_velocity_mmps = v;

  } else {
    state.linear_velocity_mmps = m_end_behavior.end_velocity_mmps;
    state.angular_velocity_dps = 0.f;
  }

  return state;
}

void Trajectory::construct_linear_trajectory(float distance_mm) {
  m_distance_mm = distance_mm;
  construct_trajectory();
}

void Trajectory::construct_arc_trajectory(float radius_mm, float angle_deg) {
  m_angular_velocity_dps = m_speed_limits.max_angular_velocity_dps;
  if (radius_mm < 0.f) {
    m_angular_velocity_dps = -m_angular_velocity_dps;
  }

  const float angle_rad = angle_deg * std::numbers::pi_v<float> / 180.f;
  // s = rθ
  m_distance_mm = std::abs(radius_mm) * angle_rad;

  construct_trajectory();
}

void Trajectory::construct_trajectory() {
  const float& v_i = m_end_behavior.start_velocity_mmps;
  const float& v_f = m_end_behavior.end_velocity_mmps;
  const float& a   = m_speed_limits.max_acceleration_mmps2;
  const float& d   = m_distance_mm;

  // Calculate maximum attainable velocity over the distance with the given
  // acceleration limit.
  //
  // This is my derivation of the equation to calculate the maximum velocity.
  //
  // v_max^2 = v_i^2 + 2a(d_rise)
  // v_f^2 = v_max^2 - 2a(d_fall)
  //
  // v_f^2 - v_max^2 = -2a(d_fall)
  // d_fall = (v_max^2 - v_f^2) / 2a
  //
  // d = d_rise + d_fall
  // d_rise = d - d_fall
  //
  // v_max^2 = v_i^2 + 2a(d - d_fall)
  // v_max^2 = v_i^2 + 2a(d - (v_max^2 - v_f^2) / 2a)
  // v_max^2 = v_i^2 + 2ad - v_max^2 + v_f^2
  // 2v_max^2 = v_i^2 + v_f^2 + 2ad
  // v_max^2 = (v_i^2 + v_f^2) / 2 + ad
  //
  // v_max = √((v_i^2 + v_f^2) / 2 + ad)
  float& v_max = m_max_velocity_mmps;
  v_max        = std::sqrt((v_i * v_i + v_f * v_f) / 2.f + a * d);

  // Make sure the max velocity is not greater than the configured max velocity.
  v_max = std::clamp(v_max, 0.f, m_speed_limits.max_velocity_mmps);

  // Calculate the distance to reach max velocity.
  //
  // v_max^2 = v_i^2 + 2a(d_rise)
  // d_rise = (v_max^2 - v_i^2) / 2a
  const float d_rise = (v_max * v_max - v_i * v_i) / (2.f * a);

  // Calculate the distance to decelerate from max velocity to the final
  // velocity.
  //
  // v_f^2 = v_max^2 - 2a(d_fall)
  // d_fall = (v_max^2 - v_f^2) / 2a
  const float d_fall = (v_max * v_max - v_f * v_f) / (2.f * a);

  // Distance to cruise at max velocity.
  const float d_cruise = d - (d_rise + d_fall);

  m_time_rise_s = (v_max - v_i) / a;
  m_time_fall_s = (v_max - v_f) / a;
  m_time_cruise_s = d_cruise / v_max;

  m_time_total_s = m_time_rise_s + m_time_cruise_s + m_time_fall_s;
}

float TrajectoryUtil::calc_ideal_arc_velocity(
    float radius_mm, const Trajectory::SpeedLimits& speed_limits) {

  // v^2 = a_c * r
  return std::sqrt(speed_limits.max_centripetal_acceleration_mmps2 * radius_mm);
}
