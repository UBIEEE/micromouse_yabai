#include "Control/robot_control.hpp"

#include "Buzzer/buzzer.hpp"
#include "Vision/vision.hpp"
#include "Drive/drive.hpp"
#include "Drive/imu.hpp"

#include "custom_stm.h"

void RobotControl::process() {
  if (m_is_next_task) {
    m_is_next_task = false;
    start_next_task();
  }

  process_current_task();
}

void RobotControl::on_connect_send_feedback() { send_current_task(); }

void RobotControl::handle_button_1() {
  // If the robot's doing something, stop it.
  if (current_task() != Task::NONE) {
    run_task(Task::NONE);
    return;
  }

  if (m_search_done) {
    arm_task(Task::MAZE_FAST_SOLVE);
  } else {
    arm_task(Task::MAZE_SEARCH);
  }
}

void RobotControl::handle_button_2() {
  // If the robot's doing something, stop it.
  if (current_task() != Task::NONE) {
    run_task(Task::NONE);
    return;
  }

  reset_maze();
}

void RobotControl::run_task(Task task) {
  if (m_task == task) return;

  m_next_task    = task;
  m_is_next_task = true;
  m_armed_task   = Task::NONE;
}

void RobotControl::reset_maze() {
  // TODO: Reset the maze...
  m_search_done = false;
}

void RobotControl::start_next_task() {
  // Reset stuff.

  Drive::get().stop();
  m_task = m_next_task;

  if (m_task != Task::NONE) {
    Drive::get().reset();
    Vision::get().set_enabled(true);
  }
  else {
    Vision::get().set_enabled(false);
  }

  const bool imu_standby = (m_task == Task::NONE);
  drive::IMU::get().set_standby_mode(imu_standby);

  // Start task.

  switch (m_task) {
  case Task::MAZE_SEARCH:
    start_task_maze_search();
    break;
  case Task::MAZE_FAST_SOLVE:
    start_task_maze_fast_solve();
    break;
  default:
    break;
  }

  send_current_task();
}

void RobotControl::end_task() {
  m_task = Task::NONE;

  if (m_task == Task::MAZE_SEARCH) {
    m_search_done = true;
  }

  send_current_task();
}

void RobotControl::arm_task(Task task) {
  m_armed_task = task;
  run_task(Task::ARMED);
}

void RobotControl::start_task_maze_search() {
  /* Buzzer::get().play_song(Buzzer::Song::BEGIN_SEARCH); */

  // TODO: Start
}

void RobotControl::start_task_maze_fast_solve() {
  /* Buzzer::get().play_song(Buzzer::Song::BEGIN_FAST_SOLVE); */

  // TODO: Start
}

void RobotControl::process_current_task() {
  switch (m_task) {
  case Task::MAZE_SEARCH:
    process_task_maze_search();
    break;
  case Task::MAZE_FAST_SOLVE:
    process_task_maze_fast_solve();
    break;
  case Task::ARMED:
    process_armed();
    break;
  default:
    break;
  }
}

void RobotControl::process_task_maze_search() {
  // TODO: Process

  /*

  if (done) {
    end_task();
  }

   */
}

void RobotControl::process_task_maze_fast_solve() {
  // TODO: Process

  /*

  if (done) {
    end_task();
  }

   */
}

void RobotControl::process_armed() {
  // TODO: Check vision sensors.
}

void RobotControl::send_current_task() {

  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_TASK_CHAR, (uint8_t*)&m_task);
}

void RobotControl_RunTask(uint8_t task) {
  if (task >= uint8_t(RobotControl::Task::_COUNT)) return;

  RobotControl::get().run_task(static_cast<RobotControl::Task>(task));
}

void RobotControl_ResetMaze(void) { RobotControl::get().reset_maze(); }

