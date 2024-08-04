#include "Basic/error_manager.hpp"

#include "custom_stm.h"

//
// Constants.
//

static constexpr uint16_t MAX_ERRORS = 64;

//
// ErrorManager functions.
//

void ErrorManager::on_connect_send_feedback() {
  for (auto error : m_errors) {
    Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_ERRORCODE_CHAR,
                               (uint8_t*)&error);
  }
}

void ErrorManager::report_error(Error error, bool fatal) {
  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_ERRORCODE_CHAR, (uint8_t*)&error);

  m_errors.push_back(error);

  if (m_errors.size() > MAX_ERRORS) {
    m_errors.pop_front();
  }

  if (fatal) error_handler();
}

void ErrorManager::error_handler() {
  __disable_irq();
  while (1) {
  }
}

#include "Basic/robot.h"

void ErrorManager_ReportError(void) {
  ErrorManager::get().report_error(ErrorManager::Error::UNKNOWN);
}
