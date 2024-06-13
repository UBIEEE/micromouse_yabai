#include "Basic/robot.h"

#include "Buzzer/buzzer.hpp"
#include "Drive/drive.hpp"
#include "Vision/vision.hpp"

#include "main.h"
#include <array>

////////////////////////////////////////////////////////////////////////////////
// Subsystems.
////////////////////////////////////////////////////////////////////////////////

static Buzzer buzzer;
static Drive drive;
static Vision vision;

static constexpr std::array<Subsystem*, 3> subsystems = {
    &buzzer,
    &drive,
    &vision,
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

//
// External interrupt callback.
//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin:
    buzzer.play_song(Buzzer::Song::HOME_DEPOT);
    // drive.set_speed(0.2, 0.2);
    vision.set_enabled(true);
    break;
  case BUTTON_2_Pin:
    buzzer.quiet();
    drive.stop();
    vision.set_enabled(false);
    break;
  case IMU_INT1_Pin:
    break;
  }
}
