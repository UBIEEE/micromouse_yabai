#pragma once

#include "Basic/constants.hpp"
#include "Maze/cell.hpp"
#include "Maze/coordinate.hpp"
#include "Maze/direction.hpp"
#include <cstring>
#include <iterator>
#include <span>

namespace maze {

//
// Represents the maze.
//
// +x is east
// +y is north
//
//    · · · · ·
//    · · · · ·
//    · · t · ·
//    · · · · ·
//    r · · · s
//
// _r_ and _s_ are start cells. The mouse starts facing north.
// _t_ is the target cell.
//
class Maze {
  Cell m_cells[Constants::MAZE_CELLS];
  uint8_t m_cell_values[Constants::MAZE_CELLS];

public:
  // Center four cells.
  inline static constexpr Coordinate GOAL_ENDPOINTS[] = {
      {7, 7}, {7, 8}, {8, 7}, {8, 8}};

  // Southwest corner cell (goal is to the right).
  inline static constexpr Coordinate WEST_START_ENDPOINTS[] = {{0, 0}};

  // Southeast corner cell (goal is to the left).
  inline static constexpr Coordinate EAST_START_ENDPOINTS[] = {{15, 0}};

  // The two possible start locations for the mouse.
  enum class StartLocation {
    WEST_OF_GOAL,
    EAST_OF_GOAL
  };

public:
  Maze();

  // Returns the start endpoints based on the start location.
  static std::span<const Coordinate>
  get_start_endpoints(StartLocation start_location) {
    switch (start_location) {
      using enum StartLocation;
    case WEST_OF_GOAL:
      return WEST_START_ENDPOINTS;
    case EAST_OF_GOAL:
      return EAST_START_ENDPOINTS;
    }
  }

  bool operator==(const Maze& other) const {
    return std::memcmp(m_cells, other.m_cells, Constants::MAZE_CELLS) == 0;
  }
  bool operator!=(const Maze& other) const { return !(*this == other); }

  // Zeroes out the maze and places boundaries. This does not need to be called
  // initially, just when the maze needs to be reset after being used.
  void reset();

  // Places a wall bounding the start cell based on the start location.
  void init_start_cell(StartLocation start_location);

  bool is_wall(Coordinate coord, Direction dir) const {
    return m_cells[coord].is_wall(dir);
  }
  bool is_exit(Coordinate coord, Direction dir) const {
    return m_cells[coord].is_exit(dir);
  }
  void set_wall(Coordinate coord, Direction dir, bool present = true);

  bool is_cell_visited(Coordinate coord) const {
    return m_cells[coord].is_visited();
  }
  void set_cell_visited(Coordinate coord) { m_cells[coord].set_visited(); }

  Cell& cell(Coordinate coord) { return m_cells[coord]; }
  Cell& operator[](Coordinate coord) { return m_cells[coord]; }

  // Returns the coordinate of a cell. The cell must be within the maze.
  Coordinate cell_coordinate(const Cell* cell) const {
    return Coordinate(std::distance(m_cells, cell));
  }

  // Returns nullptr if the neighbor is out of bounds.
  Cell* neighbor_cell(Coordinate coord, Direction direction);

  uint8_t cell_value(Coordinate coord) const {
    return m_cell_values[coord];
  }

  void set_cell_value(Coordinate coord, uint8_t value) {
    m_cell_values[coord] = value;
  }

private:
  // Places walls around the perimeter of the maze.
  void init_boundaries();
};

} // namespace maze

using maze::Maze;
