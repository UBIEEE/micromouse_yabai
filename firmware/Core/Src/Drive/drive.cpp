#include "Drive/drive.hpp"

#include <cmath>

#include "custom_stm.h"

//
// External variables.
//

// Left encoder
extern LPTIM_HandleTypeDef hlptim1; // main.c
// Right encoder
extern TIM_HandleTypeDef htim2; // main.c

//
// Constants.
//

static constexpr int8_t PWM_PERIOD = 20; // The resolution of the PWM.

// PID constants.
static constexpr float TRANSLATIONAL_KP = 0.001f;
static constexpr float TRANSLATIONAL_KI = 0.0f;
static constexpr float TRANSLATIONAL_KD = 0.0f;
static constexpr float ANGULAR_KP       = 0.1f;
static constexpr float ANGULAR_KI       = 0.0f;
static constexpr float ANGULAR_KD       = 0.0f;

//
// Drive functions.
//

Drive::Drive()
  : m_translational_left_pid(TRANSLATIONAL_KP, TRANSLATIONAL_KI,
                             TRANSLATIONAL_KD, ROBOT_UPDATE_PERIOD_S),
    m_translational_right_pid(TRANSLATIONAL_KP, TRANSLATIONAL_KI,
                              TRANSLATIONAL_KD, ROBOT_UPDATE_PERIOD_S),
    m_angular_pid(ANGULAR_KP, ANGULAR_KI, ANGULAR_KD, ROBOT_UPDATE_PERIOD_S) {}

void Drive::process() {
  update_encoders();

  // TODO: Implement PID control.
}

void Drive::set_speed(float left_mmps, float right_mmps) {
  m_target_left_vel_mmps  = left_mmps;
  m_target_right_vel_mmps = right_mmps;
}

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

void Drive::set_speed_dir_raw(uint8_t left, GPIO_PinState left_dir,
                              uint8_t right, GPIO_PinState right_dir) {

  HAL_GPIO_WritePin(MOTOR_LEFT_DIR_GPIO_Port, MOTOR_LEFT_DIR_Pin, left_dir);
  HAL_GPIO_WritePin(MOTOR_RIGHT_DIR_GPIO_Port, MOTOR_RIGHT_DIR_Pin, right_dir);

  m_pwm_pulse_left  = left;
  m_pwm_pulse_right = right;
}

void Drive::update_encoders() {
  m_time_us += 20;

  const uint16_t left_ticks  = hlptim1.Instance->CNT;
  const uint16_t right_ticks = htim2.Instance->CNT / 2;

  const Encoder::Data left_data  = m_left_encoder.update(left_ticks, m_time_us);
  const Encoder::Data right_data = m_right_encoder.update(right_ticks, m_time_us);

  m_encoder_data.left  = left_data;
  m_encoder_data.right = right_data;
}

void Drive::update_pwm() {
  if (m_pwm_counter < m_pwm_pulse_right) {
    MOTOR_RIGHT_GPIO_Port->BSRR = uint32_t(MOTOR_RIGHT_Pin); // Set
  } else {
    MOTOR_RIGHT_GPIO_Port->BRR = uint32_t(MOTOR_RIGHT_Pin); // Reset
  }

  if (m_pwm_counter < m_pwm_pulse_left) {
    MOTOR_LEFT_GPIO_Port->BSRR = uint32_t(MOTOR_LEFT_Pin); // Set
  } else {
    MOTOR_LEFT_GPIO_Port->BRR = uint32_t(MOTOR_LEFT_Pin); // Reset
  }

  m_pwm_counter++;
  m_pwm_counter %= PWM_PERIOD;
}

void Drive::send_feedback() {
  static_assert(sizeof(m_encoder_data) == 16);

  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_DATA_CHAR,
                             (uint8_t*)&m_encoder_data);
}

//
// Callbacks.
//

// TIM17 interrupt callback.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  assert_param(htim->Instance == TIM7);
  UNUSED(htim);

  Drive::get().update_encoders();

  // We can't use hardware PWM generation because of a mistake in PCB, so we
  // have to do it manually.
  Drive::get().update_pwm();
}