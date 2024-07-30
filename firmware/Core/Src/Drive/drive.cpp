#include "Drive/drive.hpp"

#include "Basic/constants.hpp"
#include "Drive/odometry.hpp"
#include "custom_stm.h"
#include <cmath>
#include <cstring>

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
static constexpr float ANGULAR_KP       = 0.001f;
static constexpr float ANGULAR_KI       = 0.00025f;
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
  using enum ControlMode;

  update_encoders();

  if (m_control_mode == IDLE) {
    m_raw_speed_data = {};

  } else if (m_control_mode == MANUAL) {
    // Raw speeds already set, so nothing to do here.

  } else {
    if (CONTROLLER == m_control_mode) {
      const auto [left, right] = m_search_controller.update();
      m_velocity_control_data  = {
           .target_linear_mmps = left,
           .target_angular_dps = right,
      };
    }

    // Update PID controllers with velocity control data to produce raw speed
    // data.
    update_pid_controllers();
  }

  // Set the motors.
  const auto& [left, right] = m_raw_speed_data;
  set_motors(left, right);
}

void Drive::reset() {
  reset_encoders();
  reset_pid_controllers();

  m_control_mode = ControlMode::IDLE;
}

void Drive::reset_encoders() {
  m_left_encoder.reset();
  m_right_encoder.reset();
}

void Drive::reset_pid_controllers() {
  m_translational_left_pid.reset();
  m_translational_right_pid.reset();
  m_angular_pid.reset();

  m_velocity_control_data = {};
}

void Drive::stop() {
  m_raw_speed_data = {};

  reset_pid_controllers();

  reset_to_mode(ControlMode::IDLE);
}

void Drive::control_speed_raw(float left_percent, float right_percent) {
  m_raw_speed_data = {
      .left  = left_percent,
      .right = right_percent,
  };

  reset_to_mode(ControlMode::MANUAL);
}

void Drive::control_speed_velocity(float linear_mmps, float angular_dps) {
  m_velocity_control_data.target_linear_mmps = linear_mmps,
  m_velocity_control_data.target_angular_dps = angular_dps,

  reset_to_mode(ControlMode::VELOCITY);
}

void Drive::begin_search() {
  m_controller = &m_search_controller;

  reset_to_mode(ControlMode::CONTROLLER);
}

void Drive::begin_solve() {
  m_controller = &m_solve_controller;

  reset_to_mode(ControlMode::CONTROLLER);
}

void Drive::reset_to_mode(ControlMode mode) {
  using enum ControlMode;

  if (CONTROLLER == mode) {
    m_controller->reset();
  }

  if (mode == m_control_mode) {
    return;
  }

  m_control_mode = mode;
}

void Drive::set_motors(float left_percent, float right_percent) {
  const int8_t left(left_percent * PWM_PERIOD);
  const int8_t right(right_percent * PWM_PERIOD);

  const bool left_dir  = std::signbit(left);
  const bool right_dir = std::signbit(right);

  const int8_t left_out  = left_dir ? -left : left;
  const int8_t right_out = right_dir ? -right : right;

  const GPIO_PinState left_dir_pin  = static_cast<GPIO_PinState>(left_dir);
  const GPIO_PinState right_dir_pin = static_cast<GPIO_PinState>(right_dir);

  set_motors_raw(left_out, left_dir_pin, right_out, right_dir_pin);
}

void Drive::set_motors_raw(uint8_t left, GPIO_PinState left_dir, uint8_t right,
                           GPIO_PinState right_dir) {

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

  const float& delta_left_mm =
      left_data.position_mm - m_drive_data.encoder.left.position_mm;
  const float& delta_right_mm =
      right_data.position_mm - m_drive_data.encoder.right.position_mm;

  m_drive_data.encoder.left  = left_data;
  m_drive_data.encoder.right = right_data;

  Odometry::update(m_drive_data.pose, delta_left_mm, delta_right_mm);
}

void Drive::update_pid_controllers() {
  auto& control_data = m_velocity_control_data;

  // Angular PID controller.
  {
    const float gyro_z_dps = IMU::get().get_angular_velocity(IMU::Axis::Z);

    control_data.final_angular_dps +=
        m_angular_pid.calculate(gyro_z_dps, control_data.target_angular_dps);
  }

  // Translational PID controllers.
  {
    const float w  = deg_to_rad(control_data.final_angular_dps);
    const float& v = control_data.target_linear_mmps;
    const float& b = Constants::ROBOT_TRACK_WIDTH_MM;

    const float v_r = v + (w * b) / 2.f;
    const float v_l = v - (w * b) / 2.f;

    control_data.final_right_speed += m_translational_right_pid.calculate(
        m_drive_data.encoder.right.velocity_mmps, v_r);

    control_data.final_left_speed += m_translational_left_pid.calculate(
        m_drive_data.encoder.left.velocity_mmps, v_l);
  }

  float final_left  = control_data.final_left_speed,
        final_right = control_data.final_right_speed;

  // Clamp the values.
  if (std::abs(final_left) < 0.05f) {
    final_left = 0.f;
  }
  if (std::abs(final_right) < 0.05f) {
    final_right = 0.f;
  }

  // Set raw speed data.
  m_raw_speed_data = {
      .left  = final_left,
      .right = final_right,
  };
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
  static_assert(sizeof(m_drive_data) == (4 * 4 + 3 * 4));

  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_DATA_CHAR,
                             reinterpret_cast<uint8_t*>(&m_drive_data));
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
