#include "Control/robot_control.hpp"

#include "Control/robot_control.h"

#include "custom_stm.h"

void RobotControl::process() {

}

void RobotControl::on_connect_send_feedback() {
  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_CURRENTTASK_CHAR, (uint8_t*)&m_current_task);
}

void RobotControl::send_feedback() {

}

void RobotControl::run_task(Task task) {
  // if (task == Task::NONE) {
  //   // TODO: End current task.
  // }

  // if (m_current_task != Task::NONE) return;

  m_current_task = task;

  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_CURRENTTASK_CHAR, (uint8_t*)&m_current_task);
}

void RobotControl_RunTask(uint8_t task) {
  if (task > 2) return;

  RobotControl::get().run_task(static_cast<RobotControl::Task>(task));
}
