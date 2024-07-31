#include "Maze/maze.hpp"
#include "Solver/flood_fill.hpp"
#include <gtest/gtest.h>

TEST(FloodFill, Test1) {
  Maze maze;
  FloodFillSolver solver(maze);
}
