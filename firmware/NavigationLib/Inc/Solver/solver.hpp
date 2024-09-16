#pragma once

#include "Maze/coordinate.hpp"
#include "Maze/maze.hpp"
#include <span>

class Solver {
  const Maze& m_maze;
 
private:
  bool done;

protected:
  Solver(Maze& maze) : m_maze(maze) {}

public:
  virtual ~Solver() = default;

  //virtual void solve(maze::CoordinateSpan endpoints) const = 0;

  void setDone(bool done) { this->done = done;}

  bool getDone() { return done; }

  bool atDestination(Coordinate current) { 
	  for (Coordinate coor : m_maze.GOAL_ENDPOINTS) {
		  if (coor == current) {
				  return true;
		  }
	  }
      return false;
  }

  void updateGrid(Coordinate coor, bool north, bool east, bool south,bool west) {
      Cell cell = m_maze.cell(coor);
      cell.set_north(north);
      cell.set_east(east);
      cell.set_south(south);
      cell.set_west(west);
  }

  virtual void iterate();
  virtual void finish();
};
