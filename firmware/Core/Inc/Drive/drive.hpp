#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/drive.h"
#include "Drive/encoder.hpp"
#include "Drive/imu.hpp"
#include "Math/pid_controller.hpp"
#include "Drive/drive.h"

#include <cstdint>

namespace drive {

class Drive : public SubsystemSingleton<Drive> {
  // Drive stuff.

  static inline uint8_t m_pwm_counter     = 0;
  static inline uint8_t m_pwm_pulse_right = 0;
  static inline uint8_t m_pwm_pulse_left  = 0;

  // Encoder stuff.

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

  enum class ControlMode {
    IDLE,
    MANUAL,
    VELOCITY,
  } m_control_mode = ControlMode::IDLE;

  struct {
    float left = 0.f;
    float right = 0.f;
  } m_raw_speed_data;

  struct {
    float linear_velocity_mmps = 0.f;
    float angular_velocity_dps = 0.f;

    float last_angular_speed = 0.f;
    float last_right_speed = 0.f;
    float last_left_speed  = 0.f;
  } m_velocity_control_data;

public:
  Drive();

  void process() override;
  void send_feedback() override;
  void on_connect_send_feedback() override;

  // Resets everything.
  void reset();
  void reset_encoders();
  void reset_pid_controllers();

  bool is_idle() const { return m_control_mode == ControlMode::IDLE; }
  void stop();

  // Set the speed of the motors (percent of max speed).
  // Values should range between [-1.0, 1.0].
  void control_speed_raw(float left_percent, float right_percent);

  // Set the target speed of the motors.
  void control_speed_velocity(float linear_mmps, float angular_dps);

  float left_encoder_position() const {
    return m_encoder_data.left.position_mm;
  }
  float right_encoder_position() const {
    return m_encoder_data.right.position_mm;
  }

private:
  static void set_motors(float left_percent, float right_percent);

  static void set_motors_raw(uint8_t left, GPIO_PinState left_dir,
                             uint8_t right, GPIO_PinState right_dir);

  void update_encoders();
  void update_pid_controllers();

private:
  friend void ::HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef*);

  // We can't use hardware PWM generation because of a mistake in PCB, so we
  // have to do it manually.
  void update_pwm();

private:
  friend void ::Drive_UpdatePIDConstants(uint8_t*);

  void update_pid_constants(float* constants);
};

} // namespace drive

using drive::Drive;
