#pragma once

#include "Basic/subsystem.hpp"

class RobotControl : public SubsystemSingleton<RobotControl> {
public:

  enum class Task : uint8_t {
    NONE = 0,
    MAZE_SEARCH = 1,
    MAZE_FAST_SEARCH = 2,
  };

private:
  Task m_current_task = Task::MAZE_FAST_SEARCH;

public:
  void process() override;
  void on_connect_send_feedback() override;
  void send_feedback() override;

  void run_task(Task task);

private:
};
