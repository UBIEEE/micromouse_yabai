#pragma once

#include "Maze/maze.hpp"
#include "Solver/solver.hpp"

class FloodFillSolver : public Solver {
public:
  FloodFillSolver(Maze& maze)
    : Solver(maze) {}

  void solve(maze::CoordinateSpan endpoints) const override;
};
