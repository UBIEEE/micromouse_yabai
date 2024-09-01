#pragma once

#include "Basic/singleton.hpp"
#include "Basic/subsystem.hpp"

#include <list>

enum class Error : uint8_t {
  UNKNOWN = 0,
  IMU_CONFIG_FAIL,
  UNREACHABLE,
};

class ErrorManager : public Subsystem, public Singleton<ErrorManager> {
  std::list<Error> m_errors;

public:
  void on_connect_send_feedback() override;

  void report_error(Error error, bool fatal = false);
  void fatal_error(Error error) { report_error(error, true); }

  void clear_errors() { m_errors.clear(); }

private:
  void error_handler();
};
