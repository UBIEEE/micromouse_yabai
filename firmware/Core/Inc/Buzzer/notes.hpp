#pragma once

#include <cstdint>

//
// Values to set the buzzer timer counter period (auto-reload register) to.
//
enum Note : uint16_t {
  REST = 0,

  // NOTE_C_4  = 0,
  // NOTE_CS_4 = 0,
  // NOTE_D_4  = 0,
  // NOTE_DS_4 = 0,
  // NOTE_E_4  = 0,
  // NOTE_F_4  = 0,
  // NOTE_FS_4 = 0,
  NOTE_G_4  = 3360,
  NOTE_GS_4 = 3180,
  NOTE_A_4  = 2995,
  NOTE_AS_4 = 2840,
  NOTE_B_4  = 2690,

  NOTE_C_5  = 2545,
  NOTE_CS_5 = 2410,
  NOTE_D_5  = 2280,
  NOTE_DS_5 = 2160,
  NOTE_E_5  = 2040,
  NOTE_F_5  = 1930,
  NOTE_FS_5 = 1830, // ?
  NOTE_G_5  = 1740, // 810
  NOTE_GS_5 = 760,
  NOTE_A_5  = 720,
  NOTE_AS_5 = 680,
  NOTE_B_5  = 640,

  // NOTE_C_6  = 0,
  NOTE_CS_6 = 570,
  NOTE_D_6  = 540,
  // NOTE_DS_6 = 510,
  NOTE_E_6 = 480, // Just a guess
};
