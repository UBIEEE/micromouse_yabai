#pragma once

#include "Basic/subsystem.hpp"

#include <list>

class ErrorManager : public SubsystemSingleton<ErrorManager> {
public:
  enum class Error : uint8_t {
    UNKNOWN = 0,
    IMU_CONFIG_FAIL,
    SOMETHING,
  };

private:
  std::list<Error> m_errors;

public:
  void on_connect_send_feedback() override;

  void report_error(Error error, bool fatal = false);
  void fatal_error(Error error) { report_error(error, true); }

  void clear_errors() { m_errors.clear(); }

private:
  void error_handler();
};
