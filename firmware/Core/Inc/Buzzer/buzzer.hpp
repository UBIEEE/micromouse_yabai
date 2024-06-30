#pragma once

#include "Basic/subsystem.hpp"

#include <cstdint>

struct SongHandle;

class Buzzer : public SubsystemSingleton<Buzzer> {
public:
  enum class Song : uint8_t {
    NONE          = 0,
    STARTUP       = 1,
    BLE_CONECT    = 2,
    BLE_DISCONECT = 3,
    HOME_DEPOT    = 4,
    NOKIA         = 5,

    _COUNT,
  };

private:
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
