#pragma once

#include "Basic/subsystem.hpp"

#include <cstdint>

class Drive : public SubsystemSingleton<Drive> {

  struct {
    float left_dist_mm = 0.f;
    float right_dist_mm = 0.f;

    float left_vel_mmps = 0.f;
    float right_vel_mmps = 0.f;
  } m_data;

public:
  void process() override;
  void send_feedback() override;

  void stop() { set_speed_dir(0, GPIO_PIN_RESET, 0, GPIO_PIN_RESET); };

  // Set the speed of the motors.
  // Values should be between -1.0 and 1.0.
  void set_speed(float left_percent, float right_percent);

private:
  void set_speed_dir(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                     GPIO_PinState right_dir);

  void update_encoders();
};
