#include "Drive/drive.hpp"

#include <cmath>

extern LPTIM_HandleTypeDef hlptim1; // Left encoder
extern TIM_HandleTypeDef htim2;     // Right encoder

static constexpr int8_t PWM_PERIOD = 10; // The resolution of the PWM.

static uint8_t s_pwm_counter     = 0;
static uint8_t s_pwm_pulse_right = 0;
static uint8_t s_pwm_pulse_left  = 0;

void Drive::process() {
  uint16_t left_enc  = hlptim1.Instance->CNT;
  uint16_t right_enc = htim2.Instance->CNT / 2;
}

void Drive::set_speed(float left_float, float right_float) {
  const int8_t left(left_float * PWM_PERIOD);
  const int8_t right(right_float * PWM_PERIOD);

  const int8_t abs_left  = std::abs(left);
  const int8_t abs_right = std::abs(right);

  const GPIO_PinState left_dir =
      (abs_left / left) > 0 ? GPIO_PIN_RESET : GPIO_PIN_SET;
  const GPIO_PinState right_dir =
      (abs_right / right) > 0 ? GPIO_PIN_RESET : GPIO_PIN_SET;

  set_speed_dir(abs_left, left_dir, abs_right, right_dir);
}

void Drive::set_speed_dir(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                          GPIO_PinState right_dir) {
  HAL_GPIO_WritePin(MOTOR_LEFT_DIR_GPIO_Port, MOTOR_LEFT_DIR_Pin, left_dir);
  HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_GPIO_Port, MOTOR_RIGHT_DIR_Pin, right_dir);

  s_pwm_pulse_left  = left;
  s_pwm_pulse_right = right;
}

//
// Timer callback.
//
// Can't use hardware PWM because mistake in PCB, so we have to do it manually.
//
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  assert_param(htim->Instance == TIM17);
  UNUSED(htim);

  if (s_pwm_counter < s_pwm_pulse_right) {
    MOTOR_RIGHT_GPIO_Port->BSRR = uint32_t(MOTOR_RIGHT_Pin); // Set
  } else {
    MOTOR_RIGHT_GPIO_Port->BRR = uint32_t(MOTOR_RIGHT_Pin); // Reset
  }

  if (s_pwm_counter < s_pwm_pulse_left) {
    MOTOR_LEFT_GPIO_Port->BSRR = uint32_t(MOTOR_LEFT_Pin); // Set
  } else {
    MOTOR_LEFT_GPIO_Port->BRR = uint32_t(MOTOR_LEFT_Pin); // Reset
  }

  s_pwm_counter++;
  s_pwm_counter %= PWM_PERIOD;
}
