#include "Drive/drive_controller.hpp"

#include "Utilities/math.hpp"

using namespace drive;

void DriveController::reset() {
  m_linear_profile.reset();
  m_linear_profile.configure(0.f, 0.f, 0.f, 0.f);

  m_linear_timer.stop();
  m_linear_timer.reset();

  m_angular_profile.reset();
  m_angular_profile.configure(0.f, 0.f, 0.f, 0.f);

  m_angular_timer.stop();
  m_angular_timer.reset();
}

void DriveController::process() {
  const float linear_elapsed_s  = m_linear_timer.elapsed_s();
  const float angular_elapsed_s = m_angular_timer.elapsed_s();

  const bool linear_done  = m_linear_profile.is_done_at(linear_elapsed_s);
  const bool angular_done = m_angular_profile.is_done_at(angular_elapsed_s);

  switch (m_motion) {
    using enum Motion;
  case FORWARD:
    if (linear_done) {
      m_motion = Motion::NONE;
    }
    break;
  case TURN:
    process_turn(linear_done, angular_done);
    break;
  case NONE:
    break;
  }

  const auto linear_sample  = m_linear_profile.sample(linear_elapsed_s);
  const auto angular_sample = m_angular_profile.sample(angular_elapsed_s);

  m_drive.control_speed_velocity(linear_sample.velocity,
                                 angular_sample.velocity);
}

void DriveController::process_turn(bool linear_done, bool angular_done) {
  switch (m_turn_state) {
    using enum TurnState;
  case LEADUP:
    if (linear_done) {
      m_turn_state = ARC;
      start_arc();
      break;
    }
    break;
  case ARC:
    if (angular_done) {
      if (m_turn_followup_distance_mm == 0.f) {
        m_motion = Motion::NONE;
        break;
      }

      m_turn_state = FOLLOWUP;

      config_linear(m_turn_followup_distance_mm);
      config_angular(0.f);
    }
    break;
  case FOLLOWUP:
    if (linear_done) {
      m_motion = Motion::NONE;
    }
    break;
  }
}

void DriveController::forward(float distance_mm, bool end_high) {
  m_motion = Motion::FORWARD;

  config_linear(distance_mm, end_high);
  config_angular(0.f);
}

void DriveController::turn(float leadup_distance_mm, TurnAngle angle,
                           float followup_distance_mm) {

  m_motion     = Motion::TURN;
  m_turn_angle = angle;

  const float angle_deg = int16_t(m_turn_angle);
  const float angle_rad = deg_to_rad(angle_deg);

  m_turn_leadup_distance_mm   = leadup_distance_mm;
  m_turn_followup_distance_mm = followup_distance_mm;
  m_turn_arc_distance_mm      = m_speeds->turn_radius_mm * angle_rad;

  if (leadup_distance_mm == 0.f) { // Jump straight to turning.
    m_turn_state = TurnState::ARC;
    start_arc();
    return;
  }

  m_turn_state = TurnState::LEADUP;

  config_linear(m_turn_leadup_distance_mm);
  config_angular(0.f);
}

void DriveController::start_arc() {
  const float angle_deg = int16_t(m_turn_angle);

  float turn_linear_velocity_mmps = m_linear_profile.final_velocity();

  const float angular_acceleration_dps2 = [&] {
    const float& v_max = m_speeds->angular_velocity_dps;
    const float t_total =
        (m_turn_arc_distance_mm / turn_linear_velocity_mmps) / 2.f;
    const float x_total = angle_deg / 2.f;

    //
    // x_accel = 0.5a(t_accel)^2
    // x_cruise = v_max(t_cruise)
    //
    // x_total = x_accel + x_cruise
    // t_total = t_accel + t_cruise
    //
    // v_max = a(t_accel)
    // t_accel = v_max / a
    //
    // t_cruise = t_total - t_accel
    // t_cruise = t_total - (v_max / a)
    //
    // x_total = 0.5a(t_accel)^2 + v(t_cruise)
    // x_total = 0.5a(v_max / a)^2 + v_max(t_total - (v_max / a))
    // x_total = 0.5(v_max)^2 / a + v_max(t_total) - (v_max)^2 / a
    // x_total - v_max(t_total) = -0.5(v_max)^2 / a
    // a = -0.5(v_max)^2 / (x_total - v_max(t_total))
    // a = (v_max)^2 / (2(v_max(t_total) - x_total))
    //
    return (v_max * v_max) / (2 * (v_max * t_total - x_total));
  }();

  config_linear(m_turn_arc_distance_mm, turn_linear_velocity_mmps,
                turn_linear_velocity_mmps);

  config_angular(angle_deg, 0.f, m_speeds->angular_velocity_dps,
                 angular_acceleration_dps2);
}

void DriveController::config_linear(float distance_mm, bool end_high) {
  const float final_velocity_mmps =
      end_high ? m_speeds->linear_velocity_mmps : 0.f;

  config_linear(distance_mm, final_velocity_mmps);
}

void DriveController::config_linear(float distance_mm,
                                    float final_velocity_mmps) {

  config_linear(distance_mm, final_velocity_mmps,
                m_speeds->linear_velocity_mmps);
}

void DriveController::config_linear(float distance_mm,
                                    float final_velocity_mmps,
                                    float max_velocity_mmps) {

  const float final_distance_mm =
      m_linear_profile.sample(m_linear_timer.elapsed_s()).distance;

  const float extra_distance_mm =
      final_distance_mm - m_linear_profile.final_distance();

  distance_mm -= extra_distance_mm;

  m_linear_profile.configure(distance_mm, final_velocity_mmps,
                             max_velocity_mmps,
                             m_speeds->linear_acceleration_mmps2);

  m_linear_timer.reset();
  m_linear_timer.start();
}

void DriveController::config_angular(float angle_deg) {
  m_angular_profile.configure(angle_deg, m_speeds->angular_velocity_dps,
                              m_speeds->angular_velocity_dps,
                              m_speeds->angular_acceleration_dps2);

  m_angular_timer.reset();
  m_angular_timer.start();
}

void DriveController::config_angular(float angle_deg, float final_velocity_dps,
                                     float max_velocity_dps,
                                     float acceleration_dps2) {

  m_angular_profile.configure(angle_deg, final_velocity_dps, max_velocity_dps,
                              acceleration_dps2);

  m_angular_timer.reset();
  m_angular_timer.start();
}
