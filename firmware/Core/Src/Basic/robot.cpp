#include "Basic/robot.h"

#include "Buzzer/buzzer.hpp"
#include "Drive/drive.hpp"

#include "main.h"
#include <array>

////////////////////////////////////////////////////////////////////////////////
// Subsystems.
////////////////////////////////////////////////////////////////////////////////

static Buzzer buzzer;
static Drive drive;

static constexpr std::array<Subsystem*, 2> subsystems = {
    &buzzer,
    &drive,
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

////////////////////////////////////////////////////////////////////////////////
// Callbacks.
////////////////////////////////////////////////////////////////////////////////

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin:
    buzzer.play_song(Buzzer::Song::HOME_DEPOT);
    drive.set_speed(0.2, 0.2);
    break;
  case BUTTON_2_Pin:
    buzzer.quiet();
    drive.stop();
    break;
  case IMU_INT1_Pin:
    break;
  }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim) {
  assert_param(htim->Instance == TIM17);
  UNUSED(htim);

  drive.pwm_update();
}
