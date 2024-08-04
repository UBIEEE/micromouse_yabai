#pragma once

#include "main.h"
#include "Basic/robot.h"

// Base class for subsystems.
// Note: Subclasses should override init() instead of using a constructor
// because the object may be created before peripherals are initialized!
class Subsystem {
public:
  virtual ~Subsystem() = default;

  Subsystem(const Subsystem&)            = delete;
  Subsystem(Subsystem&&)                 = delete;
  Subsystem& operator=(const Subsystem&) = delete;
  Subsystem& operator=(Subsystem&&)      = delete;

  virtual void init() {}
  virtual void process() {}
  virtual void on_connect_send_feedback() {}
  virtual void send_feedback() {}

protected:
  Subsystem() = default;
};
