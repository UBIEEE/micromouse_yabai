#include "Basic/robot.hpp"

#include "Basic/constants.hpp"
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
    arm_task(Task::MAZE_SLOW_SOLVE);
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

  m_navigator.stop();
  m_drive.stop();
  m_task = m_next_task;

  {
    const bool idle = (m_task == Task::NONE);

    if (!idle) {
      m_drive.reset();
      m_navigator.reset_position(Maze::start(m_start_location),
                                 maze::Direction::NORTH,
                                 Constants::RobotCellPositions::BACK_WALL_MM);
    }

    m_vision.set_enabled(!idle);
    m_drive.imu().set_standby_mode(idle);
  }

  // Start task.

  switch (m_task) {
  case Task::NONE:
    break;
  case Task::MAZE_SEARCH:
    start_task_maze_search();
    break;
  case Task::MAZE_SLOW_SOLVE:
    start_task_maze_solve(false);
    break;
  case Task::MAZE_FAST_SOLVE:
    start_task_maze_solve(true);
    break;
  case Task::TEST_DRIVE_STRAIGHT:
    start_task_test_drive_straight();
    break;
  case Task::TEST_DRIVE_LEFT_TURN:
    start_task_test_drive_left_turn();
    break;
  case Task::TEST_DRIVE_RIGHT_TURN:
    start_task_test_drive_right_turn();
    break;
  case Task::TEST_GYRO:
    start_task_test_gyro();
    break;
  case Task::ARMED:
    break;
  default:
    ErrorManager::get().fatal_error(Error::UNREACHABLE);
  }

  send_current_task();
}

void Robot::start_task_maze_search() {
  m_buzzer.play_song(Buzzer::Song::BEGIN_SEARCH);

  m_search_stage = SearchStage::START_TO_GOAL;
  m_navigator.search_to(Maze::GOAL_ENDPOINTS, m_flood_fill_solver);
}

void Robot::start_task_maze_solve(bool fast) {
  {
    using enum Buzzer::Song;
    m_buzzer.play_song(fast ? BEGIN_FAST_SOLVE : BEGIN_SLOW_SOLVE);
  }

  m_solve_stage = SolveStage::START_TO_GOAL;
  m_navigator.solve_to(Maze::GOAL_ENDPOINTS, fast);
}

void Robot::start_task_test_drive_straight() {}
void Robot::start_task_test_drive_left_turn() {}
void Robot::start_task_test_drive_right_turn() {}

void Robot::start_task_test_gyro() { m_drive.control_speed_velocity(0.f, 90.f); }

void Robot::process_current_task() {
  switch (m_task) {
    using enum Task;
  case NONE:
    break;
  case MAZE_SEARCH:
    process_task_maze_search();
    break;
  case MAZE_SLOW_SOLVE:
    process_task_maze_solve(false);
    break;
  case MAZE_FAST_SOLVE:
    process_task_maze_solve(true);
    break;
  case TEST_DRIVE_STRAIGHT:
  case TEST_DRIVE_LEFT_TURN:
  case TEST_DRIVE_RIGHT_TURN:
    process_task_test_drive();
    break;
  case TEST_GYRO:
    break;
  case ARMED:
    process_armed();
    break;
  default:
    ErrorManager::get().fatal_error(Error::UNREACHABLE);
  }
}

void Robot::process_task_maze_search() {
  using enum SearchStage;

  if (m_navigator.is_done()) {
    maze::CoordinateSpan next_target;

    // Check which stage was just finished, and set the next target accordingly.
    switch (m_search_stage) {
    case START_TO_GOAL:
      // Goal -> Outside Start
      next_target = Maze::outside_start_span(m_start_location);
      break;
    case GOAL_TO_OUTSIDE_START:
      // Outside Start -> Goal
      next_target = Maze::GOAL_ENDPOINTS;
      break;
    case OUTSIDE_START_TO_GOAL:
      // Goal -> Start
      next_target = Maze::start_span(m_start_location);
      break;
    case GOAL_TO_START:
      end_task();
      return;
    default:
      ErrorManager::get().fatal_error(Error::UNREACHABLE);
    }

    m_navigator.search_to(next_target, m_flood_fill_solver);

    m_search_stage = SearchStage(uint8_t(m_search_stage) + 1);
  }
}

void Robot::process_task_maze_solve(bool fast) {
  if (m_navigator.is_done()) {
    switch (m_solve_stage) {
      using enum SolveStage;
    case START_TO_GOAL:
      m_navigator.solve_to(Maze::start_span(m_start_location), fast);
      m_solve_stage = GOAL_TO_START;
      break;
    case GOAL_TO_START:
      end_task();
      return;
    }
  }
}

void Robot::process_task_test_drive() {}

void Robot::process_armed() {
  // TODO: Check vision sensors.
}

void Robot::send_current_task() {
  uint8_t data[2] = {uint8_t(m_task), uint8_t(m_start_location)};

  Custom_STM_App_Update_Char(CUSTOM_STM_MAIN_TASK_CHAR, data);
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

void RobotControl_RunTask(uint8_t task, uint8_t start_location) {
  using enum Robot::Task;

  if (task > uint8_t(_COUNT)) return;

  const Robot::Task final_task = Robot::Task(task);

  Robot::get().run_task(final_task);

  if (final_task == MAZE_SEARCH) {
    if (start_location >= 2) return;
    Robot::get().set_start_location(Maze::StartLocation(start_location));
  }
}

void RobotControl_ResetMaze(void) { Robot::get().reset_maze(); }
