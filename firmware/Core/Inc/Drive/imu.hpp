#pragma once

#include <cstdint>

#include "Basic/subsystem.hpp"
#include "main.h"

// ICM-42670-P
//
// Datasheet:
// https://product.tdk.com/system/files/dam/doc/product/sensor/mortion-inertial/imu/data_sheet/ds-000451-icm-42670-p.pdf
class IMU : public SubsystemSingleton<IMU> {
  static constexpr bool m_accel_enabled = true;

  bool m_init         = false;
  bool m_is_receiving = false;

  float m_gyro_conversion;
  float m_accel_conversion;
  float m_gyro_read_period;

  struct {
    float vel_dps[3];   // Angular velocity, in degrees per second.
    float angle_deg[3]; // Angle, in degrees.
  } m_gyro_data;

  struct {
    float accel_g[3]; // Acceleration, in gravities.
  } m_accel_data;

  uint8_t m_data_raw[12];

public:
  enum Axis {
    X = 0,
    Y,
    Z,
  };

public:
  void init() override;
  void send_feedback() override;

  float get_angle(Axis axis) const { return m_gyro_data.angle_deg[axis]; }
  float get_angular_velocity(Axis axis) const { return m_gyro_data.vel_dps[axis]; }
  float get_acceleration(Axis axis) const { return m_accel_data.accel_g[axis]; }

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
