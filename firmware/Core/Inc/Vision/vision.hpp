#pragma once

#include "Basic/subsystem.hpp"

#include <cstdint>

class Vision : public Subsystem {
  bool m_enabled = false;

public:
  enum Sensor : uint8_t {
    FAR_RIGHT = 0, // Channel 7
    MID_RIGHT,     // Channel 8
    MID_LEFT,      // Channel 9
    FAR_LEFT,      // Channel 10
  };

private:
  uint8_t m_raw_readings[4] = {0}; // From ADC DMA.
  uint8_t m_readings[4]     = {0};

  Sensor m_sensor = Sensor::FAR_RIGHT;

  enum class State {
    IDLE,
    WAITING,
    READING,
  };

  State m_state           = State::IDLE;
  uint8_t m_waiting_ticks = 0;

public:
  void process() override;

  void set_enabled(bool enabled) { m_enabled = enabled; };
  bool enabled() const { return m_enabled; };

  float get_reading(Sensor sensor) const { return m_readings[sensor]; };
};
