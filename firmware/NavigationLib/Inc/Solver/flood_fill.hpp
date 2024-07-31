#pragma once

#include "Solver/solver.hpp"

class FloodFillSolver : public Solver {
public:
  FloodFillSolver(Maze& maze)
    : Solver(maze) {}

  void solve(std::span<maze::Coordinate> endpoints) override;
};
