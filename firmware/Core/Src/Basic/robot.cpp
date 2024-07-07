#include "Basic/robot.h"

#include "Basic/error_manager.hpp"
#include "Buzzer/buzzer.hpp"
#include "Control/robot_control.hpp"
#include "Drive/drive.hpp"
#include "Drive/imu.hpp"
#include "Vision/vision.hpp"

#include "main.h"
#include <array>

//
// Static variables.
//

static constexpr std::array<Subsystem*, 6> s_subsystems = {
    &Buzzer::get(), &Drive::get(),        &IMU::get(),
    &Vision::get(), &RobotControl::get(), &ErrorManager::get(),
};

static bool s_ble_connected = false;

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

void Robot_OnConnect() { Buzzer::get().play_song(Buzzer::Song::BLE_CONECT); }

void Robot_DashboardAppReady() {
  for (auto s : s_subsystems) {
    s->on_connect_send_feedback();
  }

  s_ble_connected = true;
}

void Robot_OnDisconnect() {
  Buzzer::get().play_song(Buzzer::Song::BLE_DISCONECT);
  s_ble_connected = false;
}

void Robot_SendFeedback() {
  if (!s_ble_connected) return;

  for (auto s : s_subsystems) {
    s->send_feedback();
  }
}

//
// Callbacks.
//

// External interrupt callback.
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  switch (GPIO_Pin) {
  case BUTTON_1_Pin:
    Buzzer::get().play_song(Buzzer::Song::HOME_DEPOT);
    Vision::get().set_enabled(true);
    Drive::get().set_speed(1000, 1000);
    break;
  case BUTTON_2_Pin:
    Buzzer::get().quiet();
    Vision::get().set_enabled(false);
    Drive::get().stop();
    break;
  case IMU_INT1_Pin:
    IMU::get().int1_handler();
    break;
  }
}
