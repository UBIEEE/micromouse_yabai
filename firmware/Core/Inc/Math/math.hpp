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
// Constexpr Square root.
//

constexpr float sqrt_newton_raphson(float x, float curr, float prev) {
  return curr == prev ? curr
                      : sqrt_newton_raphson(x, 0.5f * (curr + x / curr), curr);
}

constexpr float sqrt_constexpr(float x) {
  return x >= 0 && x < std::numeric_limits<float>::infinity()
             ? sqrt_newton_raphson(x, x, 0.f)
             : std::numeric_limits<float>::quiet_NaN();
}

static_assert(sqrt_constexpr(4.f) == 2.f, "sqrt(4) != 2");
static_assert(sqrt_constexpr(16.f) == 4.f, "sqrt(16) != 4");
static_assert(sqrt_constexpr(25.f) == 5.f, "sqrt(25) != 5");

//
// Floating-point comparison.
//

template <typename T>
  requires std::is_floating_point_v<T>
constexpr bool float_equals(T x, T y,
                            T epsilon = std::numeric_limits<T>::epsilon()) {
  return std::abs(x - y) < epsilon;
}
