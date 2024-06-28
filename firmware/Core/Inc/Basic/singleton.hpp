#pragma once

// Singleton base class.
template <typename T>
class Singleton {
public:
  static constexpr T& get() { return s_instance; }

protected:
  Singleton() = default;

  inline static T s_instance;
};
