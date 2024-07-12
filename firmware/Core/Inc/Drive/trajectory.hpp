#pragma once

class Trajectory {
public:
  struct SpeedLimits {
    float max_velocity_mmps;      // mm/s
    float max_acceleration_mmps2; // mm/s^2

    float max_angular_velocity_dps;           // deg/s
    float max_centripetal_acceleration_mmps2; // mm/s^2
  };

  struct EndBehavior {
    float start_velocity_mmps = 0.f;
    float end_velocity_mmps   = 0.f;
  };

  struct State {
    float linear_velocity_mmps = 0.f;
    float angular_velocity_dps = 0.f;
  };

private:
  const SpeedLimits& m_speed_limits;
  const EndBehavior m_end_behavior;

  float m_distance_mm;
  float m_heading_deg;

  float m_time_rise_s;
  float m_time_cruise_s;
  float m_time_fall_s;
  float m_time_total_s;

  float m_max_velocity_mmps;

  float m_angular_velocity_dps = 0.f;

private:
  Trajectory(EndBehavior end_behavior, const SpeedLimits& speed_limits);

public:
  // Creates a linear trajectory for the given distance.
  static Trajectory create_linear(EndBehavior end_behavior, float distance_mm,
                                  const SpeedLimits& speed_limits);

  // Creates an arc trajectory with the given radius and angle.
  //
  // A positive radius creates a right turn, a negative radius creates a left
  // turn.
  static Trajectory create_arc(EndBehavior end_behavior, float radius_mm,
                               float angle_deg,
                               const SpeedLimits& speed_limits);

  const SpeedLimits& speed_limits() const { return m_speed_limits; }
  const EndBehavior& end_behavior() const { return m_end_behavior; }

  float duration_s() const { return m_time_total_s; }
  float duration_ms() const { return m_time_total_s * 1000.f; }

  State sample(float time_s);

private:
  void construct_linear_trajectory(float distance_mm);
  void construct_arc_trajectory(float radius_mm, float angle_deg);

  void construct_trajectory();
};

class TrajectoryUtil {
public:
  // Max velocity for turning radius and configured max centripetal
  // acceleration.
  static float
  calc_ideal_arc_velocity(float radius_mm,
                          const Trajectory::SpeedLimits& speed_limits);
};

