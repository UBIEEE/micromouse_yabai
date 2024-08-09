#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/drive_controller.hpp"
#include "Maze/coordinate.hpp"
#include "Maze/direction.hpp"
#include "Maze/maze.hpp"

class Navigator : public Subsystem {
  DriveController& m_drive_controller;
  Maze& m_maze;

public:
  enum class Mode {
    SEARCH,
    SLOW_SOLVE,
    FAST_SOLVE,
  };

private:
  maze::CoordinateSpan m_target;
  Mode m_mode;

  bool m_done = false;

  maze::Coordinate m_position;
  maze::Direction m_direction;

  float m_cell_distance_mm;

public:
  Navigator(DriveController& drive_controller, Maze& maze)
    : m_drive_controller(drive_controller),
      m_maze(maze) {}

  void process() override;
  void on_connect_send_feedback() override;
  void send_feedback() override;

  void reset_position(maze::Coordinate position, maze::Direction direction,
                      float cell_distance_mm);

  bool done() const { return m_done; }
  void stop() { m_done = true; }

  void navigate_to(maze::CoordinateSpan target, Mode mode = Mode::SEARCH);
};
