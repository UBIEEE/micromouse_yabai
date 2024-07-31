#pragma once

#include "Maze/constants.hpp"

#include <cstdint>
#include <utility>

namespace maze {

class Coordinate {
  uint8_t m_index;

public:
  constexpr Coordinate(uint8_t x, uint8_t y)
    : m_index(y * Constants::MAZE_WIDTH_CELLS + x) {}

  constexpr Coordinate(std::pair<uint8_t, uint8_t> coord)
    : Coordinate(coord.first, coord.second) {}

  explicit constexpr Coordinate(uint8_t index)
    : m_index(index) {}

  operator uint8_t() const { return m_index; }
  operator std::pair<uint8_t, uint8_t>() const { return to_pair(); }

  uint8_t x() const { return m_index % Constants::MAZE_WIDTH_CELLS; }
  uint8_t y() const { return m_index / Constants::MAZE_WIDTH_CELLS; }

  std::pair<uint8_t, uint8_t> to_pair() const { return {x(), y()}; }
};

} // namespace maze
