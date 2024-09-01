#include "Buzzer/buzzer.hpp"

#include "Buzzer/notes.hpp"
#include "custom_stm.h"
#include "stm32wbxx_hal.h"

//
// External variables.
//

extern TIM_HandleTypeDef htim16; // main.c

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
  m_song_handle = &m_songs[uint8_t(song)];
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

#include "Basic/robot.hpp"

void Buzzer_PlaySong(uint8_t song) {
  if (song == 0 || song >= uint8_t(Buzzer::Song::_COUNT)) {
    Robot::get().buzzer().quiet();
    return;
  }
  Robot::get().buzzer().play_song(static_cast<Buzzer::Song>(song));
}
