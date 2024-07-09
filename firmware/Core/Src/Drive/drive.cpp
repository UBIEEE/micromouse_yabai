#include "Drive/drive.hpp"

#include <cmath>
#include <cstring>

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
static constexpr float TRANSLATIONAL_KP = 0.0004616805171f;
static constexpr float TRANSLATIONAL_KI = 0.0f;
static constexpr float TRANSLATIONAL_KD = 0.0f;
static constexpr float ANGULAR_KP       = 0.0025f;
static constexpr float ANGULAR_KI       = 0.0025f;
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

  if (m_control_mode == ControlMode::VELOCITY) {
    update_pid_controllers();
  } else {
    m_left_raw_speed  = 0.f;
    m_right_raw_speed = 0.f;
  }

  set_speed_raw(m_left_raw_speed, m_right_raw_speed);
}

void Drive::set_speed(float left_mmps, float right_mmps) {
  m_target_left_vel_mmps  = left_mmps;
  m_target_right_vel_mmps = right_mmps;

  m_control_mode = ControlMode::VELOCITY;
}

void Drive::stop() {
  m_target_left_vel_mmps  = 0.f;
  m_target_right_vel_mmps = 0.f;

  m_control_mode = ControlMode::IDLE;
}

void Drive::reset_encoders() {
  m_left_encoder.reset();
  m_right_encoder.reset();
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
  const uint16_t left_ticks  = hlptim1.Instance->CNT;
  const uint16_t right_ticks = htim2.Instance->CNT / 2;

  const Encoder::Data left_data  = m_left_encoder.update(left_ticks);
  const Encoder::Data right_data = m_right_encoder.update(right_ticks);

  m_encoder_data.left  = left_data;
  m_encoder_data.right = right_data;
}

void Drive::update_pid_controllers() {
  const float angular_diff = m_angular_pid.calculate(
      IMU::get().get_angular_velocity(IMU::Axis::Z), 0.f);

  const float left_speed_diff = m_translational_left_pid.calculate(
      m_encoder_data.left.velocity_mmps, m_target_left_vel_mmps);

  const float right_speed_diff = m_translational_right_pid.calculate(
      m_encoder_data.right.velocity_mmps, m_target_right_vel_mmps);

  m_left_raw_speed += left_speed_diff;
  m_right_raw_speed += right_speed_diff;

  m_left_raw_speed -= angular_diff;
  m_right_raw_speed += angular_diff;

  if (std::abs(m_left_raw_speed) < 0.05f) {
    m_left_raw_speed = 0.f;
  }
  if (std::abs(m_right_raw_speed) < 0.05f) {
    m_right_raw_speed = 0.f;
  }
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
                             reinterpret_cast<uint8_t*>(&m_encoder_data));
}

void Drive::on_connect_send_feedback() {
  float pid_constants[6] = {m_translational_left_pid.kp(),
                            m_translational_left_pid.ki(),
                            m_translational_left_pid.kd(),
                            m_angular_pid.kp(),
                            m_angular_pid.ki(),
                            m_angular_pid.kd()};

  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_PIDCONSTANTS_CHAR,
                             reinterpret_cast<uint8_t*>(pid_constants));
}

void Drive::update_pid_constants(float* constants) {
  m_translational_left_pid.set_pid(constants[0], constants[1], constants[2]);
  m_translational_right_pid.set_pid(constants[0], constants[1], constants[2]);
  m_angular_pid.set_pid(constants[3], constants[4], constants[5]);
}

void Drive_UpdatePIDConstants(uint8_t* buf) {
  float pid_constants[6];
  std::memcpy(pid_constants, buf, sizeof(pid_constants));

  Drive::get().update_pid_constants(pid_constants);
}

//
// Callbacks.
//

// TIM17 interrupt callback.
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  assert_param(htim->Instance == TIM7);
  UNUSED(htim);

  // We can't use hardware PWM generation because of a mistake in PCB, so we
  // have to do it manually.
  Drive::get().update_pwm();
}
