#pragma once

#include "Basic/subsystem.hpp"
#include "Drive/drive.hpp"
#include "Drive/speed_config.hpp"
#include "DriveTools/trapezoid_profile.hpp"
#include "Utilities/timer.hpp"

namespace drive {

class DriveController : public Subsystem {
  Drive& m_drive;

  TrapezoidProfile m_linear_profile;
  TrapezoidProfile m_angular_profile;

  Timer m_linear_timer;
  Timer m_angular_timer;

  const SpeedConstraints* m_speeds;

  enum class Motion {
    NONE,
    FORWARD,
    TURN,
  } m_motion = Motion::NONE;

  enum class TurnState {
    LEADUP,
    ARC,
    FOLLOWUP,
  } m_turn_state;

public:
  enum class TurnAngle : int16_t {
    CW_180  = 180,
    CW_90   = 90,
    CW_45   = 45,
    CCW_45  = -45,
    CCW_90  = -90,
    CCW_180 = -180,
  };

private:
  TurnAngle m_turn_angle;

  float m_turn_leadup_distance_mm;
  float m_turn_arc_distance_mm;
  float m_turn_followup_distance_mm;

  float m_turn_arc_max_velocity;

public:
  DriveController(Drive& drive)
    : m_drive(drive) {}

  void reset();
  void set_speeds(const SpeedConstraints& speeds) { m_speeds = &speeds; }

  void process() override;

  bool is_motion_done() const { return m_motion == Motion::NONE; }

  void forward(float distance_mm, bool end_high = true);

  void turn(float leadup_distance_mm, TurnAngle angle,
            float followup_distance_mm);

  void turn(TurnAngle angle) { turn(0.f, angle, 0.f); }

private:
  void process_turn(bool linear_done, bool angular_done);

  void start_arc();

private:
  void config_linear(float distance_mm, bool end_high = true);
  void config_linear(float distance_mm, float final_velocity_mmps);
  void config_linear(float distance_mm, float final_velocity_mmps,
                     float max_velocity_mmps);

  void config_angular(float angle_deg);
  void config_angular(float angle_deg, float final_velocity_dps,
                      float max_velocity_dps, float acceleration_dps2);
};

} // namespace drive

using drive::DriveController;
