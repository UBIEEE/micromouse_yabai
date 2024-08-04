#pragma once

#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include <span>

class Solver {
public:
  virtual ~Solver() = default;

  virtual void reset() = 0;
  virtual void solve(maze::CoordinateSpan endpoints) = 0;
};
