#pragma once

#include "Basic/subsystem.hpp"
#include "Math/pid_controller.hpp"

#include <cstdint>

class Drive : public SubsystemSingleton<Drive> {

  uint16_t m_last_left_encoder_ticks  = 0;
  uint16_t m_last_right_encoder_ticks = 0;

  struct {
    float left_dist_mm  = 0.f;
    float right_dist_mm = 0.f;

    float left_vel_mmps  = 0.f;
    float right_vel_mmps = 0.f;
  } m_data;

  PIDController m_translational_left_pid;
  PIDController m_translational_right_pid;
  PIDController m_angular_pid;

public:
  Drive();

public:
  void process() override;
  void send_feedback() override;

  void hard_stop() { set_speed_dir_raw(0, GPIO_PIN_RESET, 0, GPIO_PIN_RESET); };

  // Set the speed of the motors (percent of max speed).
  // Values should range between [-1.0, 1.0].
  void set_speed_raw(float left_percent, float right_percent);

  // Set the speed of the motors (velocity in mm/s).
  void set_speed(float left_mmps, float right_mmps);

private:
  void set_speed_dir_raw(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                         GPIO_PinState right_dir);

  void update_encoders();
};
