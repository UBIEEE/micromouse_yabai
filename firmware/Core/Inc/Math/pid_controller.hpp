#pragma once

class PIDController {
  // Proportional
  const float m_kp;
  // Integral
  const float m_ki;
  // Derivative
  const float m_kd;

  // Time delta
  const float m_dt;

  const float m_min_integral;
  const float m_max_integral;

  float m_pos_error = 0.f;
  float m_vel_error = 0.f;

  float m_prev_error  = 0.f;
  float m_total_error = 0.f;

public:
  PIDController(float kp, float ki, float kd, float dt,
                float min_integral = -1.f, float max_integral = 1.f);

  float calculate(float measurement, float setpoint);

  void reset();
};
