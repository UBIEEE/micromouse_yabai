#pragma once

#include "Maze/maze.hpp"
#include "Solver/solver.hpp"

class FloodFillSolver : public Solver {
  Maze& m_maze;

public:
  FloodFillSolver(Maze& maze)
    : m_maze(maze) {}

  void reset() override;

  void solve(maze::CoordinateSpan endpoints) override;
};
