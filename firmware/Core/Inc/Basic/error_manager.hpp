#pragma once

#include "Basic/subsystem.hpp"

#include <list>

class ErrorManager : public SubsystemSingleton<ErrorManager> {
public:
  enum class Error : uint8_t {
    UNKNOWN,
    IMU_CONFIG_FAIL,
  };

private:
  std::list<Error> m_errors;

public:
  void report_error(Error error, bool fatal = false);
  void fatal_error(Error error) { report_error(error, true); }

  void clear_errors() { m_errors.clear(); }

  void get_errors(uint8_t* buf) const;

private:
  void error_handler();
};
