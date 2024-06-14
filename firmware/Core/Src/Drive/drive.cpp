#include "Drive/drive.hpp"

#include <cmath>
#include <numbers>

//
// External variables.
//

extern LPTIM_HandleTypeDef hlptim1; // Left encoder
extern TIM_HandleTypeDef htim2;     // Right encoder

//
// Constants.
//

static constexpr int8_t PWM_PERIOD = 10; // The resolution of the PWM.

static constexpr int8_t ENCODER_MAGNET_POLES = 6;
static constexpr float GEAR_RATIO            = 20.f;
static constexpr float ENCODER_TICKS_PER_ROTATION =
    (ENCODER_MAGNET_POLES * GEAR_RATIO);

static constexpr float WHEEL_DIAMETER_MM = 25.f;
static constexpr float WHEEL_CIRCUMFERENCE_MM =
    (WHEEL_DIAMETER_MM * std::numbers::pi_v<float>);

static constexpr float ENCODER_TICKS_PER_MM =
    ENCODER_TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE_MM;

static constexpr float ENCODER_MM_PER_TICK = 1.f / ENCODER_TICKS_PER_MM;

//
// Static variables.
//

static uint8_t s_pwm_counter     = 0;
static uint8_t s_pwm_pulse_right = 0;
static uint8_t s_pwm_pulse_left  = 0;

//
// Drive functions.
//

void Drive::process() { update_encoders(); }

void Drive::set_speed(float left_percent, float right_percent) {
  const int8_t left(left_percent * PWM_PERIOD);
  const int8_t right(right_percent * PWM_PERIOD);

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

void Drive::update_encoders() {
  const int8_t left_delta_ticks  = hlptim1.Instance->CNT;
  const int8_t right_delta_ticks = htim2.Instance->CNT / 2;

  const float left_delta_mm  = left_delta_ticks * ENCODER_MM_PER_TICK;
  const float right_delta_mm = right_delta_ticks * ENCODER_MM_PER_TICK;

  // Reset the encoders.
  __HAL_LPTIM_RESET_COUNTER(&hlptim1);
  __HAL_TIM_SET_COUNTER(&htim2, 0);

  // Update the distance.
  m_left_dist_mm += left_delta_mm;
  m_right_dist_mm += right_delta_mm;

  // Calculate the velocity.
  m_left_vel_mmps  = left_delta_mm / ROBOT_UPDATE_PERIOD_S;
  m_right_vel_mmps = right_delta_mm / ROBOT_UPDATE_PERIOD_S;
}

//
// Callbacks.
//

// TIM17 callback.
// We can't use hardware PWM generation because of a mistake in PCB, so we have
// to do it manually.
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
