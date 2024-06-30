#pragma once

#include "Basic/subsystem.hpp"

#include <cstdint>

class Vision : public SubsystemSingleton<Vision> {
  bool m_enabled = false;

public:
  enum Sensor : uint8_t {
    FAR_RIGHT = 0, // Channel 7
    MID_RIGHT,     // Channel 8
    MID_LEFT,      // Channel 9
    FAR_LEFT,      // Channel 10
  };

private:
  uint8_t m_raw_readings[4]; // From ADC DMA.
  uint8_t m_readings[4];

  Sensor m_sensor = Sensor::FAR_RIGHT;

  enum class State {
    IDLE,
    WAITING,
    READING,
  };

  State m_state = State::IDLE;

  volatile bool m_adc_ready = false;

public:
  void process() override;
  void send_feedback() override;

  void set_enabled(bool enabled) { m_enabled = enabled; };
  bool enabled() const { return m_enabled; };

  float get_reading(Sensor sensor) const { return m_readings[sensor]; };

private:
  void set_emitter(Sensor sensor, GPIO_PinState state);

private:
  friend void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef*);

  void read_complete_handler();
};
