#include "Drive/imu.hpp"

#include "Basic/error_manager.hpp"

#include <climits>

//
// External variables.
//

extern I2C_HandleTypeDef hi2c1; // main.c

//
// Constants.
//

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
  m_gyro_conversion = 500.f / INT16_MAX;
  buf[0] |= 0b00000110; // 800 Hz (Low Noise mode)
  status = write_register(REG_GYRO_CONFIG0, buf[0]);
  fail_if(HAL_OK != status);

  // Configure accelerometer range (Page 57).
  if constexpr (m_accel_enabled) {
    buf[0] = 0;
    buf[0] |= 0b01000000; // ±4g
    m_accel_conversion = 4.f / INT16_MAX;
    buf[0] |= 0b00000110; // 800 Hz (Low Noise mode)
    status = write_register(REG_ACCEL_CONFIG0, buf[0]);
    fail_if(HAL_OK != status);
  }

  // Turn on the sensors (Page 55).
  buf[0] = 0b00001100; // Enable gyro and enter Low Noise mode.
  // buf[0] |= 0b00000011; // Enable accelerometer and enter Low Noise mode.
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

void IMU::interrupt_handler() {
  if (!m_init) return;

  read_gyro();

  if constexpr (m_accel_enabled) {
    read_accelerometer();
  }
}

void IMU::read_gyro() {
  const int16_t x = read_axis(REG_GYRO_DATA_X1, REG_GYRO_DATA_X0);
  const int16_t y = read_axis(REG_GYRO_DATA_Y1, REG_GYRO_DATA_Y0);
  const int16_t z = read_axis(REG_GYRO_DATA_Z1, REG_GYRO_DATA_Z0);

  m_gyro_vel_dps[Axis::X] = x * m_gyro_conversion;
  m_gyro_vel_dps[Axis::Y] = y * m_gyro_conversion;
  m_gyro_vel_dps[Axis::Z] = z * m_gyro_conversion;

  // TODO: Differentiate to get angle.
}

void IMU::read_accelerometer() {
  const int16_t x = read_axis(REG_ACCEL_DATA_X1, REG_ACCEL_DATA_X0);
  const int16_t y = read_axis(REG_ACCEL_DATA_Y1, REG_ACCEL_DATA_Y0);
  const int16_t z = read_axis(REG_ACCEL_DATA_Z1, REG_ACCEL_DATA_Z0);

  m_accel_g[Axis::X] = x * m_accel_conversion;
  m_accel_g[Axis::Y] = y * m_accel_conversion;
  m_accel_g[Axis::Z] = z * m_accel_conversion;
}

int16_t IMU::read_axis(uint8_t reg_upper_byte, uint8_t reg_lower_byte) {
  uint8_t buf[1];
  int16_t val = 0;

  read_register(reg_upper_byte, buf, 1);
  val = buf[0] << 8;
  read_register(reg_lower_byte, buf, 1);
  val |= buf[0];

  return val;
}
