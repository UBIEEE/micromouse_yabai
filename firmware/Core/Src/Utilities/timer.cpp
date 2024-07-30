#include "Utilities/timer.hpp"

#include "stm32wbxx_hal.h"

void Timer::reset() { m_time_start_ms = m_time_stop_ms = HAL_GetTick(); }

void Timer::stop() {
  m_time_stop_ms = HAL_GetTick();
  m_is_running   = false;
}

uint32_t Timer::elapsed_ms() const {
  if (!m_is_running) {
    return m_time_stop_ms - m_time_start_ms;
  }

  return HAL_GetTick() - m_time_start_ms;
}
