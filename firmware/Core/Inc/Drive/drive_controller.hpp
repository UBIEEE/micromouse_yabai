#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/drive.hpp"
#include "DriveTools/trapezoid_profile.hpp"

namespace drive {

class DriveController : public Subsystem {
  Drive& m_drive;

  TrapezoidProfile m_linear_profile;
  TrapezoidProfile m_angular_profile;

public:
  DriveController(Drive& drive)
    : m_drive(drive) {}

  Drive& drive() { return m_drive; }

  void process() override;

  enum class TurnAngle {
    LEFT_180,
    LEFT_90,
    RIGHT_90,
    LEFT_45,
    RIGHT_45,
  };

  // Positive is clockwise.
  void turn(TurnAngle angle);
};

} // namespace drive

using drive::DriveController;
