#include "Navigation/search_navigator.hpp"

void SearchNavigator::reset() {
  m_flood_fill_solver.reset();

}

drive::BasicMotion SearchNavigator::next_motion() {

  return drive::BasicMotion::STOP;
}
