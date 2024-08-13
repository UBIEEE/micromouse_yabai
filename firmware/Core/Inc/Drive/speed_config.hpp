#pragma once

namespace drive {

struct SpeedConstraints {
  float linear_velocity_mmps;
  float linear_acceleration_mmps2;

  float angular_velocity_dps;
  float angular_acceleration_dps2;

  float turn_radius_mm;
  float turn_linear_velocity_mmps;
};

struct SpeedConfig {
  SpeedConstraints m_normal_speeds = {
      .linear_velocity_mmps      = 300.f,
      .linear_acceleration_mmps2 = 500.f,
      .angular_velocity_dps      = 180.f,
      .angular_acceleration_dps2 = 360.f,
      .turn_radius_mm            = 50.f,
      .turn_linear_velocity_mmps = 300.f,
  };

  SpeedConstraints m_fast_speeds = {
      .linear_velocity_mmps      = 500.f,
      .linear_acceleration_mmps2 = 1000.f,
      .angular_velocity_dps      = 180.f,
      .angular_acceleration_dps2 = 360.f,
      .turn_radius_mm            = 50.f,
      .turn_linear_velocity_mmps = 500.f,
  };
};

} // namespace drive

using drive::SpeedConfig;
