#include "Maze/maze.hpp"

using namespace maze;

Maze::Maze() { init_boundaries(); }

void Maze::reset() {
  for (std::size_t i = 0; i < Constants::Maze::TOTAL_CELLS; ++i) {
    m_cells[i].reset();
  }

  init_boundaries();
}

void Maze::init_start_cell(StartLocation start_location) {
  using enum Direction;

  if (start_location == StartLocation::WEST_OF_GOAL) {
    m_cells[0].set_wall(EAST);
  } else {
    m_cells[15].set_wall(WEST);
  }
}

void Maze::init_boundaries() {
  using enum Direction;

  for (uint8_t i = 0; i < Constants::Maze::WIDTH_CELLS; ++i) { // O(n)
    const uint8_t south = i;
    const uint8_t north = (i + (Constants::Maze::WIDTH_CELLS *
                                (Constants::Maze::WIDTH_CELLS - 1)));
    const uint8_t west  = (i * Constants::Maze::WIDTH_CELLS);
    const uint8_t east  = ((i * Constants::Maze::WIDTH_CELLS) +
                          (Constants::Maze::WIDTH_CELLS - 1));

    // (0,0) -> (15,0) have south wall.
    m_cells[south].set_wall(SOUTH);
    // (0,15) -> (15,15) have north wall.
    m_cells[north].set_wall(NORTH);
    // (0,0) -> (0,15) have west wall.
    m_cells[west].set_wall(WEST);
    // (15,0) -> (15,15) have east wall.
    m_cells[east].set_wall(EAST);
  }
}

void Maze::set_wall(Coordinate coord, Direction dir, bool present) {
  m_cells[coord].set_wall(dir, present);

  Cell* neighbor = neighbor_cell(coord, dir);
  if (neighbor) {
    neighbor->set_wall(opposite(dir), present);
  }
}

std::optional<Coordinate> Maze::neighbor_coordinate(Coordinate coord,
                                                    Direction direction) const {
  int8_t x = coord.x();
  int8_t y = coord.y();

  switch (direction) {
    using enum Direction;
  case NORTH:
    ++y;
    break;
  case EAST:
    ++x;
    break;
  case SOUTH:
    --y;
    break;
  case WEST:
    --x;
    break;
  }

  if (x < 0 || x >= Constants::Maze::WIDTH_CELLS || y < 0 ||
      y >= Constants::Maze::WIDTH_CELLS)
    return std::nullopt;

  return Coordinate(x, y);
}

Cell* Maze::neighbor_cell(Coordinate coord, Direction direction) {
  const std::optional<Coordinate> neighbor =
      neighbor_coordinate(coord, direction);
  return neighbor ? &m_cells[neighbor.value()] : nullptr;
}

Direction Maze::smallest_neighbor(Coordinate center_coord) const {
  using enum Direction;

  Direction smallest;
  uint8_t smallest_value = 0xFF;

  for (Direction d : {NORTH, EAST, SOUTH, WEST}) {
    std::optional<Coordinate> c = neighbor_coordinate(center_coord, d);
    if (!c) continue;

    uint8_t value = cell_value(*c);

    if (value <= smallest_value) {
      smallest       = d;
      smallest_value = value;
    }
  }

  return smallest;
}

bool Maze::path_between_cells(Coordinate from, Coordinate to) {
  if (from < 0 || to < 0 || from > Constants::Maze::TOTAL_CELLS ||
      to > Constants::Maze::TOTAL_CELLS) {
    return false
  }
  Cell fromCell = this->cell(from);
  Cell toCell   = this->cell(to);

  if ((from == to + 1) && (~fromCell.is_west() & ~toCell.is_east())) {
    return true;
  } else if ((from == to - 1) && (~fromCell.is_east() & ~toCell.is_west())) {
    return true;
  } else if ((from == to + Constants::Maze::WIDTH_CELLS) &&
             (~fromCell.is_north() & ~toCell.is_south())) {
    return true;
  } else if ((from == to - Constants::Maze::WIDTH_CELLS)) && (~fromCell.is_south() & ~toCell.is_north())) {
      return true;
  }
  else {
    return false;
  }
}