#pragma once

#include "Drive/motion.hpp"
#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include "Solver/flood_fill.hpp"
#include <span>

//
// Uses the flood fill algorithm to navigate to the goal.
//
class SearchNavigator {
  Maze& m_maze;
  FloodFillSolver m_flood_fill_solver;

  bool m_solver_enabled = false;

  std::span<maze::Coordinate> m_endpoints;

public:
  SearchNavigator(Maze& maze)
    : m_maze(maze),
      m_flood_fill_solver(maze) {}

  void reset();

  bool solver_enabled() const { return m_solver_enabled; }
  void set_solver_enabled(bool enabled) { m_solver_enabled = enabled; }

  void set_target(std::span<maze::Coordinate> endpoints) {
    m_endpoints = endpoints;
  }
  std::span<maze::Coordinate> target() const { return m_endpoints; }

  drive::BasicMotion next_motion();
};
