#include "DriveTools/trapezoid_profile.hpp"

#include <algorithm>
#include <cmath>

using namespace drive;

void TrapezoidProfile::start(float d, float max_velocity, float v_f, float a) {
  m_dist_total     = d;
  m_velocity_init  = m_velocity_final;
  m_velocity_final = v_f;
  m_acceleration   = a;

  const float& v_i = m_velocity_init;
  float& v_max     = m_velocity_cruise;

  // A little optimization.
  if (v_i == max_velocity && v_i == v_f) {
    v_max = max_velocity;

    m_dist_rise = m_dist_fall = 0.f;
    m_time_rise_s = m_time_fall_s = 0.f;

    m_dist_cruise  = d;
    m_time_total_s = d / max_velocity;

    return;
  }

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
  v_max = std::sqrt((v_i * v_i + v_f * v_f) / 2.f + a * d);

  // Make sure the max velocity is not greater than the configured max
  // velocity.
  v_max = std::min(v_max, max_velocity);

  if (a > 0.f) {
    // Calculate the distance to reach max velocity.
    //
    // v_max^2 = v_i^2 + 2a(d_rise)
    // d_rise = (v_max^2 - v_i^2) / 2a
    m_dist_rise = (v_max * v_max - v_i * v_i) / (2.f * a);

    // Calculate the distance to decelerate from max velocity to the final
    // velocity.
    //
    // v_f^2 = v_max^2 - 2a(d_fall)
    // d_fall = (v_max^2 - v_f^2) / 2a
    m_dist_fall = (v_max * v_max - v_f * v_f) / (2.f * a);

    m_time_rise_s = (v_max - v_i) / a;
    m_time_fall_s = (v_max - v_f) / a;

  } else {
    m_dist_rise = m_dist_fall = 0.f;
    m_time_rise_s = m_time_fall_s = 0.f;
  }

  m_dist_cruise   = d - (m_dist_rise + m_dist_fall);
  m_time_cruise_s = m_dist_cruise / v_max;

  m_time_total_s = m_time_rise_s + m_time_cruise_s + m_time_fall_s;
}

TrapezoidProfile::Sample TrapezoidProfile::sample(float t) {
  float d, v;

  const float& v_i = m_velocity_init;
  const float& v_c = m_velocity_cruise;
  const float& v_f = m_velocity_final;

  const float& a = m_acceleration;

  if (t > m_time_total_s) {
    v = v_f;
    d = m_dist_total;

  } else if (v_i == v_c && v_i == v_f) {
    v = v_i;
    d = v * t;

  } else if (t < m_time_rise_s) {
    v = v_i + a * t;
    d = v_i * t + 0.5f * a * t * t;

  } else if (t < (m_time_rise_s + m_time_cruise_s)) {
    const float t_part = (t - m_time_rise_s);

    v = v_c;
    d = m_dist_rise + v * t_part;

  } else {
    const float t_part = (t - m_time_rise_s - m_time_cruise_s);

    v = v_c - a * t_part;
    d = (m_dist_rise + m_dist_cruise) + v_c * t_part -
        0.5f * a * t_part * t_part;
  }

  return Sample {.distance = d, .velocity = v};
}
