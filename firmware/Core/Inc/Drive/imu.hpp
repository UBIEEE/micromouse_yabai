#pragma once

#include <cstdint>

#include "Basic/subsystem.hpp"
#include "main.h"

// ICM-42670-P
//
// Datasheet:
// https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000451-icm-42670-p.pdf
class IMU : public SubsystemSingleton<IMU> {
  bool m_init         = false;
  bool m_is_receiving = false;

  float m_gyro_conversion;
  float m_accel_conversion;

  float m_gyro_data_dps[3]; // Angular velocity, in degrees per second.
  float m_accel_data_g[3];  // Acceleration, in gravities.

  uint8_t m_data_raw[12];

public:
  enum Axis {
    X = 0,
    Y,
    Z,
  };

  struct Config {
    struct Gyro {
      //
      // GYRO_CONFIG0 register (Page 56).
      //

      // 6:5 GYRO_UI_FS_SEL
      enum class Range : uint8_t {
        _2000_DPS = 0b0000'0000,
        _1000_DPS = 0b0010'0000,
        _500_DPS  = 0b0100'0000,
        _250_DPS  = 0b0110'0000,
      } range = Gyro::Range::_500_DPS;

      // 3:0 GYRO_ODR
      enum class SampleRate : uint8_t {
        _1k6_HZ  = 0b0000'0101,
        _800_HZ  = 0b0000'0110,
        _400_HZ  = 0b0000'0111,
        _200_HZ  = 0b0000'1000,
        _100_HZ  = 0b0000'1001,
        _50_HZ   = 0b0000'1010,
        _25_HZ   = 0b0000'1011,
        _12_5_HZ = 0b0000'1100,
      } sample_rate = Gyro::SampleRate::_800_HZ;

      //
      // PWR_MGMT0 register (Page 55).
      //

      // 3:2 GYRO_MODE
      enum class Mode : uint8_t {
        OFF       = 0b0000'0000, // Off
        STANDBY   = 0b0000'0100, // Standby mode
        LOW_NOISE = 0b0000'1100, // Low Noise mode
      } mode = Gyro::Mode::LOW_NOISE;

    } gyro;

    struct Accelerometer {
      //
      // ACCEL_CONFIG0 register (Page 57).
      //

      // 6:5 ACCEL_UI_FS_SEL
      enum class Range : uint8_t {
        _16_G = 0b0000'0000,
        _8_G  = 0b0010'0000,
        _4_G  = 0b0100'0000,
        _2_G  = 0b0110'0000,
      } range = Accelerometer::Range::_4_G;

      // 3:0 ACCEL_ODR
      enum class SampleRate : uint8_t {
        _1k6_HZ    = 0b0000'0101, // LN mode
        _800_HZ    = 0b0000'0110, // LN mode
        _400_HZ    = 0b0000'0111, // LP or LN mode
        _200_HZ    = 0b0000'1000, // LP or LN mode
        _100_HZ    = 0b0000'1001, // LP or LN mode
        _50_HZ     = 0b0000'1010, // LP or LN mode
        _25_HZ     = 0b0000'1011, // LP or LN mode
        _12_5_HZ   = 0b0000'1100, // LP or LN mode
        _6_25_HZ   = 0b0000'1101, // LP mode
        _3_125_HZ  = 0b0000'1110, // LP mode
        _1_5625_HZ = 0b0000'1111, // LP mode
      } sample_rate = Accelerometer::SampleRate::_800_HZ;

      //
      // PWR_MGMT0 register (Page 55).
      //

      // 1:0 ACCEL_MODE
      enum class Mode : uint8_t {
        OFF       = 0b0000'0000, // Off
        STANDBY   = 0b0000'0001, // Standby mode
        LOW_POWER = 0b0000'0010, // Low Power mode
        LOW_NOISE = 0b0000'0011, // Low Noise mode
      } mode = Accelerometer::Mode::LOW_NOISE;

    } accel;
  };

private:
  Config m_config;

public:
  void set_config(const Config& config) { m_config = config; }

  void init() override;
  void send_feedback() override;

  void set_standby_mode(bool on_standby);

  float get_angular_velocity(Axis axis) const { return m_gyro_data_dps[axis]; }
  float get_acceleration(Axis axis) const { return m_accel_data_g[axis]; }

private:
  HAL_StatusTypeDef write_register(uint8_t reg, uint8_t value);
  HAL_StatusTypeDef read_register(uint8_t reg, uint8_t* buf, uint8_t len = 1);

private:
  friend void HAL_GPIO_EXTI_Callback(uint16_t);

  // External interrupt from the IMU when data is ready.
  void int1_handler();

  friend void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef*);

  // DMA read complete handler.
  void read_complete_handler();
};
