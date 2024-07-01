#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/imu.hpp"
#include "Drive/encoder.hpp"
#include "Math/pid_controller.hpp"

#include <cstdint>

class Drive : public SubsystemSingleton<Drive> {
  // Drive stuff.

  uint8_t m_pwm_counter     = 0;
  uint8_t m_pwm_pulse_right = 0;
  uint8_t m_pwm_pulse_left  = 0;

  // Encoder stuff.

  uint64_t m_time_us = 0;

  Encoder m_left_encoder;
  Encoder m_right_encoder;

  struct {
    Encoder::Data left;
    Encoder::Data right;
  } m_encoder_data;

  // Control stuff.

  PIDController m_translational_left_pid;
  PIDController m_translational_right_pid;
  PIDController m_angular_pid;

  float m_target_left_vel_mmps  = 0.f;
  float m_target_right_vel_mmps = 0.f;

public:
  Drive();

public:
  void process() override;
  void send_feedback() override;

  void hard_stop() { set_speed_dir_raw(0, GPIO_PIN_RESET, 0, GPIO_PIN_RESET); };

  // Set the speed of the motors (velocity in mm/s).
  void set_speed(float left_mmps, float right_mmps);

private:
  // Set the speed of the motors (percent of max speed).
  // Values should range between [-1.0, 1.0].
  void set_speed_raw(float left_percent, float right_percent);

  void set_speed_dir_raw(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                         GPIO_PinState right_dir);

private:
  friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);

  void update_encoders();

  // We can't use hardware PWM generation because of a mistake in PCB, so we
  // have to do it manually.
  void update_pwm();
};
