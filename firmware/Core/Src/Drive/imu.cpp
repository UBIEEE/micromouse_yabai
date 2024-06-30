#include "Drive/imu.hpp"

#include "Basic/error_manager.hpp"

#include "custom_stm.h"

#include <climits>

//
// External variables.
//

extern I2C_HandleTypeDef hi2c1; // main.c

//
// Constants.
//

static constexpr float GYRO_VELOCITY_COMPENSATION = 1.2f;

static constexpr uint8_t IMU_ADDR = (0x68 << 1); // 7-bit address, so shift left
static constexpr uint8_t IMU_WHO_AM_I = 0x67;

static constexpr uint8_t REG_WHO_AM_I      = 0x75;
static constexpr uint8_t REG_PWR_MGMT0     = 0x1F;
static constexpr uint8_t REG_INT_CONFIG    = 0x06;
static constexpr uint8_t REG_INT_SOURCE0   = 0x2B;
static constexpr uint8_t REG_GYRO_CONFIG0  = 0x20;
static constexpr uint8_t REG_ACCEL_CONFIG0 = 0x21;

static constexpr uint8_t REG_ACCEL_DATA_X1 = 0x0B;
static constexpr uint8_t REG_ACCEL_DATA_X0 = 0x0C;
static constexpr uint8_t REG_ACCEL_DATA_Y1 = 0x0D;
static constexpr uint8_t REG_ACCEL_DATA_Y0 = 0x0E;
static constexpr uint8_t REG_ACCEL_DATA_Z1 = 0x0F;
static constexpr uint8_t REG_ACCEL_DATA_Z0 = 0x10;

static constexpr uint8_t REG_GYRO_DATA_X1 = 0x11;
static constexpr uint8_t REG_GYRO_DATA_X0 = 0x12;
static constexpr uint8_t REG_GYRO_DATA_Y1 = 0x13;
static constexpr uint8_t REG_GYRO_DATA_Y0 = 0x14;
static constexpr uint8_t REG_GYRO_DATA_Z1 = 0x15;
static constexpr uint8_t REG_GYRO_DATA_Z0 = 0x16;

static constexpr uint32_t I2C_TIMEOUT = 100;

//
// IMU Functions.
//

void IMU::init() {
  if (m_init) return;

  const auto fail_if = [](bool condition) {
    if (condition) {
      ErrorManager::get().fatal_error(ErrorManager::Error::IMU_CONFIG_FAIL);
    }
  };

  HAL_StatusTypeDef status;
  uint8_t buf[1];

  // Check WHO_AM_I register to verify I2C communication.
  status = read_register(REG_WHO_AM_I, buf, 1);
  fail_if(HAL_OK != status);
  fail_if(IMU_WHO_AM_I != buf[0]);

  // Configure gyro range (Page 56).
  buf[0] = 0;
  buf[0] |= 0b01000000; // ±500 dps
  m_gyro_conversion = (500.f / INT16_MAX) * GYRO_VELOCITY_COMPENSATION;
  buf[0] |= 0b00000110; // 800 Hz
  m_gyro_read_period = (1.f / 800);

  status = write_register(REG_GYRO_CONFIG0, buf[0]);
  fail_if(HAL_OK != status);

  // Configure accelerometer range (Page 57).
  if constexpr (m_accel_enabled) {
    buf[0] = 0;
    buf[0] |= 0b01000000; // ±4g
    m_accel_conversion = (4.f / INT16_MAX);
    buf[0] |= 0b00000110; // 800 Hz (Low Noise mode)

    status = write_register(REG_ACCEL_CONFIG0, buf[0]);
    fail_if(HAL_OK != status);
  }

  // Turn on the sensors (Page 55).
  buf[0] = 0b00001100; // Enable gyro and enter Low Noise mode.
  if constexpr (m_accel_enabled) {
    buf[0] |= 0b00000011; // Enable accelerometer and enter Low Noise mode.
  }
  status = write_register(REG_PWR_MGMT0, buf[0]);
  fail_if(HAL_OK != status);

  // Configure INT1 (Page 63).
  buf[0] = 0b00001000; // Data Ready interrupt routed to INT1.
  status = write_register(REG_INT_SOURCE0, buf[0]);
  fail_if(HAL_OK != status);

  // Configure INT1 (Page 50).
  buf[0] = 0b00000010; // Set INT1 drive circuit to push-pull.
  status = write_register(REG_INT_CONFIG, buf[0]);
  fail_if(HAL_OK != status);

  m_init = true;
}

HAL_StatusTypeDef IMU::write_register(uint8_t reg, uint8_t value) {
  uint8_t data[2] = {reg, value};

  return HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDR, data, 2, I2C_TIMEOUT);
}

HAL_StatusTypeDef IMU::read_register(uint8_t reg, uint8_t* buf, uint8_t len) {
  assert_param(len > 0);
  assert_param(buf != nullptr);

  HAL_StatusTypeDef status;

  // Write register address.
  status = HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDR, &reg, 1, I2C_TIMEOUT);
  if (status != HAL_OK) return status;

  // Read data.
  status = HAL_I2C_Master_Receive(&hi2c1, IMU_ADDR, buf, len, I2C_TIMEOUT);
  return status;
}

void IMU::int1_handler() {
  if (!m_init || m_is_receiving) return;

  HAL_StatusTypeDef status;

  uint8_t addr = m_accel_enabled ? REG_ACCEL_DATA_X1 : REG_GYRO_DATA_X1;

  // Write register address.
  status = HAL_I2C_Master_Transmit(&hi2c1, IMU_ADDR, &addr, 1, I2C_TIMEOUT);
  if (status != HAL_OK) return;

  uint8_t* data = m_accel_enabled ? m_data_raw : (m_data_raw + 6);
  uint16_t size = m_accel_enabled ? 12 : 6;

  // Read data.
  status = HAL_I2C_Master_Receive_DMA(&hi2c1, IMU_ADDR, data, size);
  if (status != HAL_OK) return;

  m_is_receiving = true;
}

void IMU::read_complete_handler() {
  if (!m_init || !m_is_receiving) return;

  if constexpr (m_accel_enabled) {
    const int16_t accel_x = (m_data_raw[0] << 8) | m_data_raw[1];
    const int16_t accel_y = (m_data_raw[2] << 8) | m_data_raw[3];
    const int16_t accel_z = (m_data_raw[4] << 8) | m_data_raw[5];

    m_accel_data.accel_g[Axis::X] = accel_x * m_accel_conversion;
    m_accel_data.accel_g[Axis::Y] = accel_y * m_accel_conversion;
    m_accel_data.accel_g[Axis::Z] = accel_z * m_accel_conversion;
  }

  const int16_t gyro_x = (m_data_raw[6] << 8) | m_data_raw[7];
  const int16_t gyro_y = (m_data_raw[8] << 8) | m_data_raw[9];
  const int16_t gyro_z = (m_data_raw[10] << 8) | m_data_raw[11];

  m_gyro_data.vel_dps[Axis::X] = gyro_x * m_gyro_conversion;
  m_gyro_data.vel_dps[Axis::Y] = gyro_y * m_gyro_conversion;
  m_gyro_data.vel_dps[Axis::Z] = gyro_z * m_gyro_conversion;

  // Differentiate angular velocity to get angle.

  m_gyro_data.angle_deg[Axis::X] +=
      int32_t(m_gyro_data.vel_dps[Axis::X]) * m_gyro_read_period;

  m_gyro_data.angle_deg[Axis::Y] +=
      int32_t(m_gyro_data.vel_dps[Axis::Y]) * m_gyro_read_period;

  m_gyro_data.angle_deg[Axis::Z] +=
      int32_t(m_gyro_data.vel_dps[Axis::Z]) * m_gyro_read_period;

  m_is_receiving = false;
}

void IMU::send_feedback() {
  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_GYRODATA_CHAR,
                             reinterpret_cast<uint8_t*>(&m_gyro_data));
  Custom_STM_App_Update_Char(CUSTOM_STM_DRIVE_ACCELDATA_CHAR,
                             reinterpret_cast<uint8_t*>(&m_accel_data));
}

//
// Callbacks.
//

// I2C DMA interrupt callback.
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* hi2c) {
  assert_param(hi2c != nullptr);
  assert_param(hi2c == &hi2c1);
  UNUSED(hi2c);

  IMU::get().read_complete_handler();
}
