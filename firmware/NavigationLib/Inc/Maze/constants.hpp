#pragma once

#include <cstdint>

namespace maze {

class Constants {
public:
  static constexpr uint8_t MAZE_WIDTH_CELLS = 16;
  static constexpr uint16_t MAZE_CELLS = MAZE_WIDTH_CELLS * MAZE_WIDTH_CELLS;
};

} // namespace maze
