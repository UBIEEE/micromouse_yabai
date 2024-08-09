#include "Navigation/navigator.hpp"

void Navigator::process() {
  if (m_done) return;

  /*
  if (finished) {
    m_done = true;
    m_drive_controller.drive().stop();
  }
  */
}

void Navigator::reset_position(maze::Coordinate position,
                               maze::Direction direction,
                               float cell_distance_mm) {
  m_position         = position;
  m_direction        = direction;
  m_cell_distance_mm = cell_distance_mm;
}

void Navigator::navigate_to(maze::CoordinateSpan target, Mode mode) {
  m_done = false;

  m_target = target;
  m_mode   = mode;
}

void Navigator::on_connect_send_feedback() {
  //
}

void Navigator::send_feedback() {
  //
}
