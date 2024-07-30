#pragma once

#include <cstdint>

class Timer {
  uint32_t m_time_start_ms = 0;
  uint32_t m_time_stop_ms  = 0;

  bool m_is_running = false;

public:
  void reset();

  void start() { m_is_running = true; }
  void stop();

  uint32_t elapsed_ms() const;
  float elapsed_s() const { return elapsed_ms() / 1000.f; }
};
