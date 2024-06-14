#pragma once

#include "main.h"

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

protected:
  Subsystem() = default;
};

// Singleton base class for subsystems.
template <typename T>
class SubsystemSingleton : public Subsystem {
public:
  static constexpr T& get() { return s_instance; }

protected:
  SubsystemSingleton() = default;

  inline static T s_instance;
};
