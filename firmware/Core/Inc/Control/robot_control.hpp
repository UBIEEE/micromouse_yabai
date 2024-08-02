#pragma once

#include "Basic/subsystem.hpp"
#include "Control/robot_control.h"

class RobotControl : public SubsystemSingleton<RobotControl> {
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
  void process() override;
  void on_connect_send_feedback() override;

  Task current_task() const {
    if (m_is_next_task) return m_next_task;
    return m_task;
  }

  void handle_button_1();
  void handle_button_2();

private:
  friend void ::RobotControl_RunTask(uint8_t);
  friend void ::RobotControl_ResetMaze(void);

  void run_task(Task task);

  // Erases all knowledge of the maze.
  void reset_maze();

private:
  // Signals that the current task is done, and performs any necessary cleanup.
  void end_task();

  void arm_task(Task task);

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

