#include "Solver/flood_fill.hpp"

void FloodFillSolver::finish(maze::CoordinateSpan endpoints) const {
  // TODO: Implement flood fill algorithm.
  (void)endpoints;
}

void FloodFillSolver::fill(maze::CoordinateSpan endpoints) { 
	for (int i = 0; i < Constants::Maze::TOTAL_CELLS; i++) {
		m_maze.cell_value[i] = 694;
	}
    std::queue<Coordinate> coordinate_queue;
    for (Coordinate coor : endpoints) {
        queue.push(coor);
        m_maze.set_cell_value(coor, 0);
	}
    while (!coordinate_queue.empty()) {
        Coordinate popped_coordinate = coordinate_queue.pop();
        uint8_t new_value             = m_maze.cell_value(popped_coordinate);
        for (Direction d : {NORTH, EAST, SOUTH, WEST}) {
          std::optional<Coordinate> c = m_maze.neighbor_coordinate(center_coord, d);
          if (!c) continue;
          if (m_maze.path_between_cells(popped_coordinate, c.value()) {
                m_maze.set_cell_value(c, new_value + 1);
                coordinate_queue.add(c);
          }
        }
    }
}

void FloodFillSolver::iterate(maze::CoordinateSpan endpoints) { 
    if (this->done) {
    
    } else if(this->atDestination(this.get_current_coordinate()){
        this.setDone(true);
    }
    else {
        Coordinate coordinate = this.get_current_coordinate();
        Cell c = m_maze.cell(coordinate).set_visited();
        this->updateGrid(coordinate, false, false, false, false); //need to change these to be ir sensor inputs
        this.fill(endpoints);
        for (Direction d : {NORTH, EAST, SOUTH, WEST}) {
          if (m_maze.path_between_cells(coordinate, m_maze.neighbor_coordinate(d)) && 
              m_maze.m_cell_values(coordinate) - m_maze.m_cell_values(m_maze.neighbor_coordinate(d)) == 1) {
              this.set_current_coordinate(m_maze.neighbor_coordinate(d));
              //add stuff here for robot to move
              return;
          }
        }

    }
}

void FloodFillSolver::finish(maze::CoordinateSpan endpoints) {
    this.setDone(false);
    while (!this.getDone()) {
        this->iterate(endpoints);
    }
}
