#pragma once

#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include <span>

class Solver {
  const Maze& m_maze;

protected:
  Solver(Maze& maze) : m_maze(maze) {}

public:
  virtual ~Solver() = default;

  virtual void solve(maze::CoordinateSpan endpoints) const = 0;
};
