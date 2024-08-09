#pragma once

namespace drive {

class TrapezoidProfile {
  float m_velocity_init;
  float m_velocity_cruise;
  float m_velocity_final = 0.f;

  float m_acceleration;

  float m_dist_rise;
  float m_dist_cruise;
  float m_dist_fall;

  float m_dist_total;

  float m_time_rise_s;
  float m_time_cruise_s;
  float m_time_fall_s;

  float m_time_total_s;

public:
  void start(float distance, float max_velocity, float final_velocity,
             float acceleration);

  float duration_s() const { return m_time_total_s; }

  struct Sample {
    float distance;
    float velocity;
  };

  Sample sample(float time_s);
};

} // namespace drive
