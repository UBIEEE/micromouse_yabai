#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/Controller/continuous_controller.hpp"
#include "Drive/Controller/incremental_controller.hpp"
#include "Drive/drive.h"
#include "Drive/encoder.hpp"
#include "Drive/imu.hpp"
#include "Drive/motion.hpp"
#include "Drive/pose.hpp"
#include "Drive/trajectory.hpp"
#include "Math/pid_controller.hpp"
#include <cstdint>
#include <queue>

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
    struct {
      Encoder::Data left;
      Encoder::Data right;
    } encoder;

    Pose pose;
  } m_drive_data;

  // Control stuff.

  PIDController m_translational_left_pid;
  PIDController m_translational_right_pid;
  PIDController m_angular_pid;

  enum class ControlMode {
    IDLE,
    MANUAL,
    VELOCITY,
    CONTROLLER,
  } m_control_mode = ControlMode::IDLE;

  struct {
    float left  = 0.f;
    float right = 0.f;
  } m_raw_speed_data;

  struct {
    float target_linear_mmps = 0.f;
    float target_angular_dps = 0.f;

    float final_angular_dps = 0.f;
    float final_right_speed = 0.f;
    float final_left_speed  = 0.f;
  } m_velocity_control_data;

  IncrementalController m_search_controller;
  ContinuousController m_solve_controller;

  DriveController* m_controller = nullptr;

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

  //
  // Search control functions.
  //
  // Control the drivetrain during search runs, where future movements are
  // unknown, and the robot drives relatively slowly.
  //

  void begin_search();

  //
  // Solve control functions.
  //
  // Control the drivetrain during solve runs, where the full path to the goal
  // is already known. The robot will (attempt to) drive at full speed.
  //

  void begin_solve();

private:
private:
  // Performs relevant reset actions when transitioning to a new mode.
  void reset_to_mode(ControlMode new_mode);

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
