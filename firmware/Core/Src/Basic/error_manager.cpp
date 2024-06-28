#include "Basic/error_manager.hpp"

#include "Basic/error_manager.h"

//
// Constants.
//

static constexpr uint16_t MAX_ERRORS = 64;

//
// ErrorManager functions.
//

void ErrorManager::report_error(Error error, bool fatal) {
  m_errors.push_back(error);

  if (m_errors.size() > MAX_ERRORS) {
    m_errors.pop_front();
  }

  // TODO: Send error to dashboard.

  if (fatal) error_handler();
}

void ErrorManager::get_errors(uint8_t* buf) const {
  assert_param(buf != nullptr);

  uint8_t i = 0;
  for (auto error : m_errors) {
    buf[i++] = static_cast<uint8_t>(error);
  }
}

void ErrorManager::error_handler() {

  __disable_irq();
  while (1) {
  }
}

void ErrorManager_ReportError(void) {
  ErrorManager::get().report_error(ErrorManager::Error::UNKNOWN);
}
