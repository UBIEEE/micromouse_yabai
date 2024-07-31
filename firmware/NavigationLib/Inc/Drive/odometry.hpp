#pragma once

#include "Drive/pose.hpp"

namespace drive {

class Odometry {
public:
  static void update(Pose& pose, const float& delta_left_mm,
                     const float& delta_right_mm);
};

} // namespace drive
