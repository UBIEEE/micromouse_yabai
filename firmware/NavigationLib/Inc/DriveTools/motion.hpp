#pragma once

#include <cstdint>

namespace drive {

enum class BasicMotion : uint8_t {
  STOP,

  // Once cell forward.
  FORWARD,

  // 180˚ turn in place.
  ROTATE_IN_PLACE,

  TURN_LEFT,
  TURN_RIGHT,
};

} // namespace drive
