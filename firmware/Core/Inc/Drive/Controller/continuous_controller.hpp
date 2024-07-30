#pragma once

#include "Drive/Controller/controller.hpp"

namespace drive {

class ContinuousController : public DriveController {
public:
  void reset() override;
  Trajectory::State update() override;
};

} // namespace drive
