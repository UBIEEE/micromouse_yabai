#pragma once

#include <cstdint>

class Constants {
public:
  class RobotDimensions {
  public:
    static constexpr float LENGTH_MM = 100.f;
    static constexpr float WIDTH_MM  = 70.f;

    static constexpr float CENTER_TO_FRONT_MM = 55.f;
    static constexpr float CENTER_TO_BACK_MM  = 45.f;

    // Distance between the left and right wheels.
    static constexpr float TRACK_WIDTH_MM = 50.4f;
  };

  class Maze {
  public:
    static constexpr uint8_t WIDTH_CELLS  = 16;
    static constexpr uint16_t TOTAL_CELLS = WIDTH_CELLS * WIDTH_CELLS;
  };

  class Cell {
  public:
    static constexpr float WIDTH_MM          = 180.f;
    static constexpr float WALL_THICKNESS_MM = 12.f;

    static constexpr float FREE_WIDTH_MM = WIDTH_MM - WALL_THICKNESS_MM;
  };

  class RobotCellPositions {
  public:
    static constexpr float BACK_WALL_MM =
        (RobotDimensions::CENTER_TO_BACK_MM + Cell::WALL_THICKNESS_MM);

    static constexpr float CENTERED_MM = Cell::WIDTH_MM / 2.f;

    static constexpr float SENSING_SPOT_MM = CENTERED_MM + 55.f;
  };
};
