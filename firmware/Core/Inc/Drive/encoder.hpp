#pragma once

#include <cstdint>

class Encoder {
  uint16_t m_last_ticks = 0;

  int64_t m_ticks  = 0;
  float m_velocity = 0.f; // Ticks per second.

  uint64_t m_last_tick_us = 0; // Microseconds since the last tick.

public:
  struct Data {
    float dist_mm  = 0.f;
    float vel_mmps = 0.f;
  };

public:
  Data update(uint16_t ticks, uint64_t time_us);

private:
  int32_t calc_delta_ticks(uint16_t current, uint16_t last);
};
