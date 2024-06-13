#pragma once

#include "main.h"

class Subsystem {
public:
  Subsystem() = default;
  virtual ~Subsystem() = default;

  virtual void init() {}
  virtual void process() = 0;

  Subsystem(const Subsystem&) = delete;
  Subsystem(Subsystem&&) = delete;
  Subsystem& operator=(const Subsystem&) = delete;
  Subsystem& operator=(Subsystem&&) = delete;
};
