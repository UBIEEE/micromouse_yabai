#include "Drive/encoder.hpp"

#include <cmath>
#include <numbers>

//
// Constants.
//

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

//
// Encoder functions.
//

Encoder::Data Encoder::update(uint16_t ticks, uint64_t time_us) {
  const int32_t delta_ticks = calc_delta_ticks(ticks, m_last_ticks);
  m_last_ticks              = ticks;

  m_ticks += delta_ticks;

  const int32_t delta_us = (time_us - m_last_tick_us);
  if (delta_ticks) {
    m_velocity     = delta_ticks / (delta_us * 1e-6f);
    m_last_tick_us = time_us;
  } else if (delta_us > 100'000) {
    m_velocity = 0;
  }

  return {m_ticks * ENCODER_TICKS_PER_MM, m_velocity * ENCODER_TICKS_PER_MM};
}

int32_t Encoder::calc_delta_ticks(uint16_t current, uint16_t last) {
  int32_t delta_ticks = current - last;

  const bool signbit            = std::signbit(delta_ticks);
  const int32_t abs_delta_ticks = signbit ? -delta_ticks : delta_ticks;

  // If the delta is big, there must have been an overflow.
  if (abs_delta_ticks > INT16_MAX) {
    // Add 0xFFFF when deep negative, subtract when positive.
    delta_ticks += UINT16_MAX * (signbit ? 1 : -1);
  }

  return delta_ticks;
}
