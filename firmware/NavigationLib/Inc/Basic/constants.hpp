#pragma once

#include <cstdint>

class Constants {
public:
  static constexpr float CELL_WIDTH_MM = 180.f;

  static constexpr uint8_t MAZE_WIDTH_CELLS = 16;
  static constexpr uint16_t MAZE_CELLS = MAZE_WIDTH_CELLS * MAZE_WIDTH_CELLS;

  static constexpr float ROBOT_LENGTH_MM = 100.f;
  static constexpr float ROBOT_WIDTH_MM  = 70.f;

  // Distance between the left and right wheels.
  static constexpr float ROBOT_TRACK_WIDTH_MM = 50.4f;
};
