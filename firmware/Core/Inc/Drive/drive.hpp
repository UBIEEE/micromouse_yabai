#pragma once

#include "Basic/subsystem.hpp"

class Drive : public Subsystem {
  uint8_t pwm_counter     = 0;
  uint8_t pwm_pulse_right = 0;
  uint8_t pwm_pulse_left  = 0;

public:
  void process() override;

  void stop() { set_speed_dir(0, GPIO_PIN_RESET, 0, GPIO_PIN_RESET); };

  //
  // Set the speed of the motors.
  // Values should be between -1.0 and 1.0.
  //
  void set_speed(float left, float right);

private:
  void set_speed_dir(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                     GPIO_PinState right_dir);

private:
  friend void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim);

  void pwm_update(); // 50kHz
};
