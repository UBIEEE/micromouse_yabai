#pragma once

#include "Basic/subsystem.hpp"

class RobotControl : public SubsystemSingleton<RobotControl> {
public:
  enum class Task : uint8_t {
    NONE             = 0,
    MAZE_SEARCH      = 1,
    MAZE_FAST_SEARCH = 2,
    DRIVE_10CM       = 3,

    _COUNT
  };

private:
  Task m_current_task = Task::NONE;

  Task m_next_task    = Task::NONE;
  bool m_is_next_task = false;

public:
  void process() override;
  void on_connect_send_feedback() override;

  void run_task(Task task);
  void task_done();

private:
  void start_task_maze_search();
  void start_task_maze_fast_search();
  void start_task_drive_10cm();

  void process_task_maze_search();
  void process_task_maze_fast_search();
  void process_task_drive_10cm();

  void send_current_task();
};
