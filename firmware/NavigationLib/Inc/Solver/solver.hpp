#pragma once

#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include <span>

class Solver {
protected:
  Maze& m_maze;

public:
  Solver(Maze& maze)
    : m_maze(maze) {}

  virtual ~Solver() = default;

  virtual void solve(std::span<maze::Coordinate> endpoints) = 0;
};
