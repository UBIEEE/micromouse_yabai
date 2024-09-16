#pragma once

#include "Maze/maze.hpp"
#include "Solver/solver.hpp"

class FloodFillSolver : public Solver {

private:
  Coordinate current_coordinate;

public:
  FloodFillSolver(Maze& maze)
    : Solver(maze) {}

  void finish(maze::CoordinateSpan endpoints) const override;
  void iterate(maze::CoordinateSpan endpoints) const override;
  void fill(maze::CoordinateSpan endpoints); 
  Coordinate get_current_coordinate() {return current_coordinate;}
  void set_current_coordinate(Coordinate coordinate) {current_coordinate = coordinate;}
};
