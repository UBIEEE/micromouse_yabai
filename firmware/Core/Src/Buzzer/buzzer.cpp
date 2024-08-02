#include "Buzzer/buzzer.hpp"

#include "Buzzer/buzzer.h"

#include "Buzzer/notes.hpp"

#include "stm32wbxx_hal.h"
#include <span>

#include "custom_stm.h"

//
// External variables.
//

extern TIM_HandleTypeDef htim16; // main.c

//
// Types.
//

struct SongHandle {
  std::span<const Note> notes;
  uint16_t ticks_per_note;
  bool pause_between_notes = true;
};

//
// Constants.
//
// clang-format off

// Startup tone.

static constexpr uint16_t SONG_STARTUP_NOTE_LENGTH_MS = 250;

static constexpr Note SONG_STARTUP_NOTES[] = {
    NOTE_D_5,
    NOTE_D_6,
};

// BLE connect tone.

static constexpr uint16_t SONG_BLE_CONNECT_NOTE_LENGTH_MS = 250;

static constexpr Note SONG_BLE_CONNECT_NOTES[] = {
    NOTE_E_5,
    NOTE_G_5,
    NOTE_E_6,
};

// BLE disconnect tone.

static constexpr uint16_t SONG_BLE_DISCONNECT_NOTE_LENGTH_MS = 250;

static constexpr Note SONG_BLE_DISCONNECT_NOTES[] = {
  NOTE_E_5,
  NOTE_G_5,
  NOTE_G_4
};

// Home Depot theme song.

static constexpr uint16_t SONG_HOME_DEPOT_NOTE_LENGTH_MS = 250;

static constexpr Note SONG_HOME_DEPOT_NOTES[] = {
    // Intro
    NOTE_A_4, NOTE_A_4, NOTE_D_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_C_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_G_4, NOTE_A_4,
    NOTE_A_4, NOTE_A_4, NOTE_D_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_C_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_G_4, NOTE_A_4,
    NOTE_A_4, NOTE_A_4, NOTE_D_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_C_5, NOTE_A_4, REST, NOTE_A_4, REST, NOTE_A_4, NOTE_G_4, NOTE_A_4,

    // Transition
    REST, NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_D_6, REST,

    // Loop
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_C_5, NOTE_A_4, NOTE_G_4, NOTE_D_6, REST,

    // End
    NOTE_A_4, NOTE_D_5, NOTE_A_4, NOTE_A_4,
};

// Nokia ringtone.

static constexpr uint16_t SONG_NOKIA_NOTE_LENGTH_MS = 180;

static constexpr Note SONG_NOKIA_NOTES[] = {
    NOTE_E_6,  NOTE_D_6, NOTE_FS_5, NOTE_FS_5, NOTE_GS_5, NOTE_GS_5,
    NOTE_CS_6, NOTE_B_5, NOTE_D_5,  NOTE_D_5,  NOTE_E_5,  NOTE_E_5,
    NOTE_B_5,  NOTE_A_5, NOTE_CS_5, NOTE_CS_5, NOTE_E_5,  NOTE_E_5,
    NOTE_A_5,  NOTE_A_5, NOTE_A_5,  NOTE_A_5,  REST, REST,
    NOTE_E_6,  NOTE_D_6, NOTE_FS_5, NOTE_FS_5, NOTE_GS_5, NOTE_GS_5,
    NOTE_CS_6, NOTE_B_5, NOTE_D_5,  NOTE_D_5,  NOTE_E_5,  NOTE_E_5,
    NOTE_B_5,  NOTE_A_5, NOTE_CS_5, NOTE_CS_5, NOTE_E_5,  NOTE_E_5,
    NOTE_A_5,  NOTE_A_5, NOTE_A_5,  NOTE_A_5,  REST,
};

// All songs.
static constexpr SongHandle SONGS[] = {
    // NONE
    {},

    // STARTUP
    {SONG_STARTUP_NOTES, SONG_STARTUP_NOTE_LENGTH_MS / ROBOT_UPDATE_PERIOD_MS},

    // BLE_CONNECT
    {SONG_BLE_CONNECT_NOTES, SONG_BLE_CONNECT_NOTE_LENGTH_MS / ROBOT_UPDATE_PERIOD_MS},

    // BLE_DISCONNECT
    {SONG_BLE_DISCONNECT_NOTES, SONG_BLE_DISCONNECT_NOTE_LENGTH_MS / ROBOT_UPDATE_PERIOD_MS},

    // HOME_DEPOT
    {SONG_HOME_DEPOT_NOTES, SONG_HOME_DEPOT_NOTE_LENGTH_MS / ROBOT_UPDATE_PERIOD_MS},

    // NOKIA
    {SONG_NOKIA_NOTES, SONG_NOKIA_NOTE_LENGTH_MS / ROBOT_UPDATE_PERIOD_MS, false},
};

// clang-format on

//
// Buzzer functions.
//

void Buzzer::init() { play_song(Buzzer::Song::STARTUP); }

void Buzzer::process() {
  if (!m_song_handle) return;

  if (m_should_stop) {
    end_song();
    m_should_stop = false;
    return;
  }

  if (m_note_index == 0 && m_note_ticks == 0) {
    m_is_playing = true;
    Custom_STM_App_Update_Char(CUSTOM_STM_MUSIC_ISPLAYING_CHAR, &m_is_playing);
  }

  if (m_note_ticks++ == 0) {
    const Note note = m_song_handle->notes[m_note_index];
    set_note(note);
  }

  // End of a note.
  if (m_note_ticks == m_song_handle->ticks_per_note) {
    m_note_ticks = 0;

    if (m_song_handle->pause_between_notes) {
      // Add a small rest to separate notes.
      set_note(0);
    }

    // Check if this is the last note in the song.
    if (++m_note_index == m_song_handle->notes.size()) {
      end_song();
    }
  }
}

void Buzzer::on_connect_send_feedback() {
  Custom_STM_App_Update_Char(CUSTOM_STM_MUSIC_ISPLAYING_CHAR, &m_is_playing);
}

void Buzzer::play_song(Song song) {
  m_song_handle = &SONGS[static_cast<uint8_t>(song)];
  m_note_index  = 0;
  m_note_ticks  = 0;
  m_should_stop = false;
}

void Buzzer::set_note(uint32_t counter_period) {
  htim16.Instance->ARR = counter_period;

  // 50% duty cycle for a square wave.
  htim16.Instance->CCR1 = counter_period / 2;
}

void Buzzer::end_song() {
  set_note(0);
  if (m_song_handle != nullptr) {
    m_is_playing  = false;
    m_song_handle = nullptr;
    Custom_STM_App_Update_Char(CUSTOM_STM_MUSIC_ISPLAYING_CHAR, &m_is_playing);
  }
}

void Buzzer_PlaySong(uint8_t song) {
  if (song == 0 || song >= uint8_t(Buzzer::Song::_COUNT)) {
    Buzzer::get().quiet();
    return;
  }
  Buzzer::get().play_song(static_cast<Buzzer::Song>(song));
}
