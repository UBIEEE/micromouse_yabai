#include "Drive/drive.hpp"

#include <cmath>
#include <numbers>

#include "custom_stm.h"

//
// External variables.
//

extern LPTIM_HandleTypeDef hlptim1; // Left encoder
extern TIM_HandleTypeDef htim2;     // Right encoder

//
// Constants.
//

static constexpr int8_t PWM_PERIOD = 10; // The resolution of the PWM.

// Encoder constants.
static constexpr int8_t ENCODER_MAGNET_POLES = 6;
static constexpr float GEAR_RATIO            = 20.f;
static constexpr float ENCODER_TICKS_PER_ROTATION =
    (ENCODER_MAGNET_POLES * GEAR_RATIO);

static constexpr float WHEEL_DIAMETER_MM = 25.f;
static constexpr float WHEEL_CIRCUMFERENCE_MM =
    (WHEEL_DIAMETER_MM * std::numbers::pi_v<float>);

static constexpr float ENCODER_TICKS_PER_MM =
    (ENCODER_TICKS_PER_ROTATION / WHEEL_CIRCUMFERENCE_MM);
static constexpr float ENCODER_MM_PER_TICK = (1.f / ENCODER_TICKS_PER_MM);

// PID constants.
static constexpr float TRANSLATIONAL_KP = 0.1f;
static constexpr float TRANSLATIONAL_KI = 0.0f;
static constexpr float TRANSLATIONAL_KD = 0.0f;
static constexpr float ANGULAR_KP       = 0.1f;
static constexpr float ANGULAR_KI       = 0.0f;
static constexpr float ANGULAR_KD       = 0.0f;

//
// Static variables.
//

static uint8_t s_pwm_counter     = 0;
static uint8_t s_pwm_pulse_right = 0;
static uint8_t s_pwm_pulse_left  = 0;

//
// Drive functions.
//

Drive::Drive()
  : m_translational_left_pid(TRANSLATIONAL_KP, TRANSLATIONAL_KI,
                             TRANSLATIONAL_KD, ROBOT_UPDATE_PERIOD_S),
    m_translational_right_pid(TRANSLATIONAL_KP, TRANSLATIONAL_KI,
                              TRANSLATIONAL_KD, ROBOT_UPDATE_PERIOD_S),
    m_angular_pid(ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ROBOT_UPDATE_PERIOD_S) {}

void Drive::process() { update_encoders(); }

void Drive::set_speed_raw(float left_percent, float right_percent) {
  const int8_t left(left_percent * PWM_PERIOD);
  const int8_t right(right_percent * PWM_PERIOD);

  const bool left_dir  = std::signbit(left);
  const bool right_dir = std::signbit(right);

  const int8_t left_out  = left_dir ? -left : left;
  const int8_t right_out = right_dir ? -right : right;

  const GPIO_PinState left_dir_pin  = static_cast<GPIO_PinState>(left_dir);
  const GPIO_PinState right_dir_pin = static_cast<GPIO_PinState>(right_dir);

  set_speed_dir_raw(left_out, left_dir_pin, right_out, right_dir_pin);
}

void Drive::set_speed(float left_mmps, float right_mmps) {}

void Drive::set_speed_dir_raw(uint8_t left, GPIO_PinState left_dir,
                              uint8_t right, GPIO_PinState right_dir) {

  HAL_GPIO_WritePin(MOTOR_LEFT_DIR_GPIO_Port, MOTOR_LEFT_DIR_Pin, left_dir);
  HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_GPIO_Port, MOTOR_RIGHT_DIR_Pin, right_dir);

  s_pwm_pulse_left  = left;
  s_pwm_pulse_right = right;
}

void Drive::update_encoders() {
  // Encoders are configured to overflow at 0xFFFF.
  const uint16_t left_ticks  = hlptim1.Instance->CNT;
  const uint16_t right_ticks = htim2.Instance->CNT / 2;

  const auto calc_delta_ticks = [](const auto current, const auto last) {
    int32_t delta_ticks = current - last;

    const bool signbit            = std::signbit(delta_ticks);
    const int32_t abs_delta_ticks = signbit ? -delta_ticks : delta_ticks;

    // If the delta is big, there must have been an overflow.
    if (abs_delta_ticks > INT16_MAX) {
      // Add 0xFFFF when deep negative, subtract when positive.
      delta_ticks += UINT16_MAX * (signbit ? 1 : -1);
    }

    return delta_ticks;
  };

  const int32_t left_delta_ticks =
      calc_delta_ticks(left_ticks, m_last_left_encoder_ticks);
  const int32_t right_delta_ticks =
      calc_delta_ticks(right_ticks, m_last_right_encoder_ticks);

  m_last_left_encoder_ticks  = left_ticks;
  m_last_right_encoder_ticks = right_ticks;

  const float left_delta_mm  = left_delta_ticks * ENCODER_MM_PER_TICK;
  const float right_delta_mm = right_delta_ticks * ENCODER_MM_PER_TICK;

  // Update the distance.
  m_data.left_dist_mm += left_delta_mm;
  m_data.right_dist_mm += right_delta_mm;

  // Calculate the velocity.
  m_data.left_vel_mmps  = left_delta_mm / ROBOT_UPDATE_PERIOD_S;
  m_data.right_vel_mmps = right_delta_mm / ROBOT_UPDATE_PERIOD_S;
}

void Drive::send_feedback() {
  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_DATA_CHAR, (uint8_t*)&m_data);
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
