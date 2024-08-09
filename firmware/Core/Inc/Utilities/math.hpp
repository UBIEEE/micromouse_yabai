#pragma once

#include <cmath>
#include <limits>
#include <numbers>

//
// Conversions.
//

constexpr float deg_to_rad(float deg) {
  return deg * (std::numbers::pi_v<float> / 180.f);
}

constexpr float rad_to_deg(float rad) {
  return rad * (180.f / std::numbers::pi_v<float>);
}

//
// Floating-point comparison.
//

template <typename T>
  requires std::is_floating_point_v<T>
constexpr bool float_equals(T x, T y,
                            T epsilon = std::numeric_limits<T>::epsilon()) {
  return std::abs(x - y) < epsilon;
}
