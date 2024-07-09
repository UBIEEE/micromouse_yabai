#include "Control/robot_control.hpp"

#include "Control/robot_control.h"
#include "Drive/drive.hpp"
#include "Drive/imu.hpp"

#include "custom_stm.h"

void RobotControl::process() {
  if (m_is_next_task) {
    m_is_next_task = false;

    // Perform reset stuff.

    Drive::get().stop();
    Drive::get().reset_encoders();
    m_current_task = m_next_task;

    const bool imu_standby = (m_current_task == Task::NONE);
    IMU::get().set_standby_mode(imu_standby);

    // Start the new tasks.

    switch (m_current_task) {
    case Task::MAZE_SEARCH:
      start_task_maze_search();
      break;
    case Task::MAZE_FAST_SEARCH:
      start_task_maze_fast_search();
      break;
    case Task::DRIVE_10CM:
      start_task_drive_10cm();
      break;
    default:
      break;
    }

    send_current_task();
  }

  // Process the current task.

  switch (m_current_task) {
  case Task::MAZE_SEARCH:
    process_task_maze_search();
    break;
  case Task::MAZE_FAST_SEARCH:
    process_task_maze_fast_search();
    break;
  case Task::DRIVE_10CM:
    process_task_drive_10cm();
    break;
  default:
    break;
  }
}

void RobotControl::on_connect_send_feedback() { send_current_task(); }

void RobotControl::run_task(Task task) {
  if (m_current_task == task) return;

  m_next_task    = task;
  m_is_next_task = true;
}

void RobotControl::task_done() {
  m_current_task = Task::NONE;

  send_current_task();
}

void RobotControl::start_task_maze_search() {}

void RobotControl::start_task_maze_fast_search() {}

void RobotControl::start_task_drive_10cm() {
  Drive::get().set_speed(500.f, 500.f);
}

void RobotControl::process_task_maze_search() {}

void RobotControl::process_task_maze_fast_search() {}

void RobotControl::process_task_drive_10cm() {

  // A very simple implementation for now.
  // TODO: Add acceleration and deceleration...

  const bool left_done  = Drive::get().left_encoder_position() >= 500.f;
  const bool right_done = Drive::get().right_encoder_position() >= 500.f;

  if (left_done && right_done) {
    Drive::get().stop();
    task_done();
  } else if (left_done) {
    Drive::get().set_speed(0.f, 500.f);
  } else if (right_done) {
    Drive::get().set_speed(500.f, 0.f);
  }
}

void RobotControl::send_current_task() {

  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_CURRENTTASK_CHAR,
                             (uint8_t*)&m_current_task);
}

void RobotControl_RunTask(uint8_t task) {
  if (task > uint8_t(RobotControl::Task::_COUNT)) return;

  RobotControl::get().run_task(static_cast<RobotControl::Task>(task));
}
