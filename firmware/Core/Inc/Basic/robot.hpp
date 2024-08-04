#pragma once

#include "Basic/singleton.hpp"

#include "Maze/maze.hpp"
#include "Navigation/search_navigator.hpp"

#include "Basic/error_manager.hpp"
#include "Buzzer/buzzer.hpp"
#include "Drive/drive.hpp"
#include "Drive/imu.hpp"
#include "Vision/vision.hpp"

#include <array>

class Robot : public Singleton<Robot> {
  Maze m_maze;
  SearchNavigator m_search_navigator {m_maze};

  Buzzer m_buzzer;
  Vision m_vision;
  Drive m_drive {m_search_navigator};

  const std::array<Subsystem*, 4> m_subsystems = {
      &ErrorManager::get(), &m_buzzer, &m_vision, &m_drive
  };

  bool m_ble_connected = false;

public:
  Buzzer& buzzer() { return m_buzzer; }
  Vision& vision() { return m_vision; }
  Drive& drive() { return m_drive; }

//
// General robot control stuff.
//
private:
  friend void ::Robot_Init(void);
  friend void ::Robot_Update(void);
  friend void ::Robot_OnConnect(void);
  friend void ::Robot_DashboardAppReady(void);
  friend void ::Robot_OnDisconnect(void);
  friend void ::Robot_SendFeedback(void);

  void init();
  void update();
  void on_connect();
  void dashboard_app_ready();
  void on_disconnect();
  void send_feedback();

//
// Task management stuff.
//
public:
  enum class Task : uint8_t {
    NONE            = 0,
    MAZE_SEARCH     = 1,
    MAZE_FAST_SOLVE = 2,
    /* MAZE_SLOW_SOLVE = 3, */
    ARMED,
    _COUNT,
  };

private:
  Task m_task = Task::NONE;

  Task m_next_task    = Task::NONE;
  bool m_is_next_task = false;

  Task m_armed_task = Task::NONE;

  bool m_search_done = false;

public:
  Task current_task() const {
    if (m_is_next_task) return m_next_task;
    return m_task;
  }

private:
  friend void ::HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

  void handle_button_1();
  void handle_button_2();

private:
  friend void ::RobotControl_RunTask(uint8_t task);
  friend void ::RobotControl_ResetMaze(void);

  void arm_task(Task task);
  void run_task(Task task);

  // Signals that the current task is done, and performs any necessary cleanup.
  void end_task();

  // Erases all knowledge of the maze.
  void reset_maze();

private:
  void start_next_task();

  void start_task_maze_search();
  void start_task_maze_fast_solve();

  void process_current_task();

  void process_task_maze_search();
  void process_task_maze_fast_solve();
  void process_armed();

private:
  void send_current_task();
};
