#pragma once

#include "Drive/trajectory.hpp"

namespace drive {

class DriveController {
public:
  virtual ~DriveController() = default;

  virtual void reset() = 0;
  virtual Trajectory::State update() = 0;
};

} // namespace drive
