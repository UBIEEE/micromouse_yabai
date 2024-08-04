#include "Basic/robot.hpp"

#include "custom_stm.h"
#include "main.h"
#include <array>

void Robot::init() {
  for (auto s : m_subsystems) {
    s->init();
  }
}

void Robot::update() {
  // Task management.
  if (m_is_next_task) {
    m_is_next_task = false;
    start_next_task();
  }
  process_current_task();

  // Process subsystems.
  for (auto s : m_subsystems) {
    s->process();
  }
}

void Robot::on_connect() { m_buzzer.play_song(Buzzer::Song::BLE_CONECT); }

void Robot::dashboard_app_ready() {
  send_current_task();

  for (auto s : m_subsystems) {
    s->on_connect_send_feedback();
  }

  m_ble_connected = true;
}

void Robot::on_disconnect() {
  m_buzzer.play_song(Buzzer::Song::BLE_DISCONECT);
  m_ble_connected = false;
}

void Robot::send_feedback() {
  if (!m_ble_connected) return;

  for (auto s : m_subsystems) {
    s->send_feedback();
  }
}

void Robot::handle_button_1() {
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

void Robot::handle_button_2() {
  // If the robot's doing something, stop it.
  if (current_task() != Task::NONE) {
    run_task(Task::NONE);
    return;
  }

  reset_maze();
}

void Robot::arm_task(Task task) {
  m_armed_task = task;
  run_task(Task::ARMED);
}

void Robot::run_task(Task task) {
  if (m_task == task) return;

  m_next_task    = task;
  m_is_next_task = true;
  m_armed_task   = Task::NONE;
}

void Robot::end_task() {
  m_task = Task::NONE;

  if (m_task == Task::MAZE_SEARCH) {
    m_search_done = true;
  }

  send_current_task();
}

void Robot::reset_maze() {
  m_maze.reset();
  m_search_done = false;
}

void Robot::start_next_task() {
  // Reset stuff.

  m_drive.stop();
  m_task = m_next_task;

  {
    const bool idle = (m_task == Task::NONE);

    if (!idle) {
      m_drive.reset();
    }

    m_vision.set_enabled(!idle);
    m_drive.imu().set_standby_mode(idle);
  }

  // Start task.

  switch (m_task) {
  case Task::MAZE_SEARCH:
    start_task_maze_search();
    break;
  case Task::MAZE_SLOW_SOLVE:
    start_task_maze_slow_solve();
    break;
  case Task::MAZE_FAST_SOLVE:
    start_task_maze_fast_solve();
    break;
  default:
    break;
  }

  send_current_task();
}

void Robot::start_task_maze_search() {
  /* m_buzzer.play_song(Buzzer::Song::BEGIN_SEARCH); */

  m_search_navigator.set_solver_enabled(true);
  m_search_navigator.set_targets(get_search_targets());

  m_drive.begin_incremental_control();
}

void Robot::start_task_maze_slow_solve() {
  /* m_buzzer.play_song(Buzzer::Song::BEGIN_SLOW_SOLVE); */

  m_search_navigator.set_solver_enabled(false);
  m_search_navigator.set_targets(get_solve_targets());

  m_drive.begin_incremental_control();
}

void Robot::start_task_maze_fast_solve() {
  /* m_buzzer.play_song(Buzzer::Song::BEGIN_FAST_SOLVE); */

  // m_drive.begin_continuous_control();
}

void Robot::process_current_task() {
  switch (m_task) {
  case Task::MAZE_SEARCH:
    process_task_maze_search();
    break;
  case Task::MAZE_SLOW_SOLVE:
    process_task_maze_slow_solve();
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

void Robot::process_task_maze_search() {
  // TODO: Process

  /*
  if (done) {
    end_task();
  }
  */
}

void Robot::process_task_maze_slow_solve() {
  // TODO: Process

  /*
  if (done) {
    end_task();
  }
  */
}

void Robot::process_task_maze_fast_solve() {
  // TODO: Process

  /*
  if (done) {
    end_task();
  }
  */
}

void Robot::process_armed() {
  // TODO: Check vision sensors.
}

void Robot::send_current_task() {
  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_TASK_CHAR, (uint8_t*)&m_task);
}

// External interrupt callback.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin: // (BOOT0)
    Robot::get().handle_button_1();
    break;
  case BUTTON_2_Pin:
    Robot::get().handle_button_2();
    break;
  case IMU_INT1_Pin:
    Robot::get().drive().imu().int1_handler();
    break;
  }
}

#include "Basic/robot.h"

void Robot_Init(void) { Robot::get().init(); }
void Robot_Update(void) { Robot::get().update(); }
void Robot_OnConnect(void) { Robot::get().on_connect(); }
void Robot_DashboardAppReady(void) { Robot::get().dashboard_app_ready(); }
void Robot_OnDisconnect(void) { Robot::get().on_disconnect(); }
void Robot_SendFeedback(void) { Robot::get().send_feedback(); }

void RobotControl_RunTask(uint8_t task) {
  if (task >= uint8_t(Robot::Task::_COUNT)) return;
  Robot::get().run_task(Robot::Task(task));
}

void RobotControl_ResetMaze(void) { Robot::get().reset_maze(); }
