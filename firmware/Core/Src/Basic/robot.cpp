#include "Basic/robot.h"

#include "Buzzer/buzzer.hpp"
#include "Drive/drive.hpp"
#include "Vision/vision.hpp"

#include "main.h"
#include <array>

//
// Static variables.
//

static constexpr std::array<Subsystem*, 3> s_subsystems = {
  &Buzzer::get(),
  &Drive::get(),
  &Vision::get(),
};

//
// Robot functions.
//

void Robot_Init() {
  for (auto s : s_subsystems) {
    s->init();
  }
}

void Robot_Update() {
  for (auto s : s_subsystems) {
    s->process();
  }
}

//
// Callbacks.
//

// External interrupt callback.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin:
    Buzzer::get().play_song(Buzzer::Song::NOKIA);
    // Drive::get().set_speed(-0.5, -0.5);
    // Vision::get().set_enabled(true);
    break;
  case BUTTON_2_Pin:
    Buzzer::get().quiet();
    // Drive::get().stop();
    // Vision::get().set_enabled(false);
    break;
  case IMU_INT1_Pin:
    break;
  }
}
