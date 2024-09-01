#pragma once

#include "Basic/subsystem.hpp"
#include "Buzzer/notes.hpp"

#include <cstdint>
#include <span>

class Buzzer : public Subsystem {
public:
  enum class Song : uint8_t {
    NONE = 0,
    STARTUP,
    BLE_CONECT,
    BLE_DISCONECT,

    HOME_DEPOT = 4,
    NOKIA      = 5,

    BEGIN_SEARCH,
    BEGIN_FAST_SOLVE,
    BEGIN_SLOW_SOLVE,

    _COUNT,
  };

private:
  struct SongHandle {
    std::span<const Note> notes;
    uint16_t ticks_per_note;
    bool pause_between_notes = true;
  };

private:
  static const SongHandle m_songs[uint8_t(Song::_COUNT)]; // songs.cpp

  const SongHandle* m_song_handle = nullptr;
  uint16_t m_note_index           = 0;
  uint16_t m_note_ticks           = 0;

  bool m_should_stop   = false;
  uint8_t m_is_playing = false;

public:
  void init() override;
  void process() override;
  void on_connect_send_feedback() override;

  // Starts playing a song from the beginning.
  void play_song(Song song);

  // Makes a song stop playing.
  void quiet() { m_should_stop = true; }

  bool is_playing() const { return (m_song_handle != nullptr); }

private:
  // Makes the buzzer play a note by setting the timer counter period
  // (auto-reload register) and the pulse width (compare register) to
  // generate a square wave of the desired frequency.
  void set_note(uint32_t counter_period);

  void end_song();
};
