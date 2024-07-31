#pragma once

#include "Maze/direction.hpp"
#include <cstdint>

namespace maze {

//
// Represents a cell in the maze.
// Contains data for the walls and whether the cell has been visited.
//
// 1 byte in size.
//
class Cell final {
  union {
    uint8_t m_data;
    struct {
      uint8_t m_north : 1;
      uint8_t m_east : 1;
      uint8_t m_south : 1;
      uint8_t m_west : 1;
      uint8_t m_visited : 4;
    };
  };

public:
  Cell()
    : m_data(0x00) {}

  Cell(bool north, bool east, bool south, bool west)
    : m_north(north),
      m_east(east),
      m_south(south),
      m_west(west),
      m_visited(0) {}

  ~Cell() = default;

  bool operator==(const Cell& other) const { return m_data == other.m_data; }
  bool operator!=(const Cell& other) const { return m_data != other.m_data; }

  void reset() { m_data = 0x00; }

  bool is_north() const { return m_north; }
  bool is_east() const { return m_east; }
  bool is_south() const { return m_south; }
  bool is_west() const { return m_west; }

  bool is_wall(Direction dir) const {
    const uint8_t mask = 1 << static_cast<uint8_t>(dir);
    return (m_data & mask) != 0;
  }

  bool is_exit(Direction dir) const {
    const uint8_t mask = 1 << static_cast<uint8_t>(dir);
    return (m_data & mask) == 0;
  }

  void set_north(bool value = true) { m_north = value; }
  void set_east(bool value = true) { m_east = value; }
  void set_south(bool value = true) { m_south = value; }
  void set_west(bool value = true) { m_west = value; }

  void set_wall(Direction dir, bool value = true) {
    const uint8_t mask = 1 << static_cast<uint8_t>(dir);

    // Clear the bit
    m_data &= ~mask;
    // Set the bit
    if (value) m_data |= mask;
  }

  bool is_visited() const { return m_visited != 0; }
  void set_visited() noexcept { m_visited = 1; }
};

static_assert(sizeof(Cell) == sizeof(uint8_t));

} // namespace maze
