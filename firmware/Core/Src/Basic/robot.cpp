#include "Basic/robot.h"

#include "Buzzer/buzzer.hpp"

#include "main.h"
#include <array>

////////////////////////////////////////////////////////////////////////////////
// Subsystems.
////////////////////////////////////////////////////////////////////////////////

static Buzzer buzzer;

static constexpr std::array<Subsystem*, 1> subsystems = {
    &buzzer,
};

////////////////////////////////////////////////////////////////////////////////
// Robot functions.
////////////////////////////////////////////////////////////////////////////////

void Robot_Init() {
  for (auto s : subsystems) {
    s->init();
  }
}

void Robot_Update() {
  for (auto s : subsystems) {
    s->process();
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin:
    buzzer.play_song(Buzzer::Song::HOME_DEPOT);
    break;
  case BUTTON_2_Pin:
    buzzer.quiet();
    break;
  case IMU_INT1_Pin:
    break;
  }
}
