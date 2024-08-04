#pragma once

#include "Drive/motion.hpp"
#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include "Solver/flood_fill.hpp"
#include <queue>
#include <span>

//
// Uses the flood fill algorithm to navigate to the goal.
//
class SearchNavigator {
  Maze& m_maze;
  FloodFillSolver m_flood_fill_solver;

  bool m_solver_enabled = false;

  std::queue<maze::CoordinateSpan> m_targets_queue;

public:
  SearchNavigator(Maze& maze)
    : m_maze(maze),
      m_flood_fill_solver(maze) {}

  bool solver_enabled() const { return m_solver_enabled; }
  void set_solver_enabled(bool enabled) { m_solver_enabled = enabled; }

  void set_targets(const std::span<const maze::CoordinateSpan>& targets) {
    m_targets_queue = {};
    for (const auto& target : targets) {
      m_targets_queue.push(target);
    }
  }

  drive::BasicMotion next_motion(maze::Coordinate current_position);
};
