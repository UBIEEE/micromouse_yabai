#pragma once

#include "Basic/subsystem.hpp"

#include <cstdint>

struct SongHandle;

class Buzzer : public SubsystemSingleton<Buzzer> {
public:
  enum class Song : uint8_t {
    NONE       = 0,
    STARTUP    = 1,
    HOME_DEPOT = 2,
  };

private:
  const SongHandle* m_song_handle = nullptr;
  uint16_t m_note_index           = 0;
  uint16_t m_note_ticks           = 0;

public:
  void init() override;
  void process() override;

  //
  // Starts playing a song from the beginning.
  //
  void play_song(Song song);

  void quiet();

  bool is_playing() const { return (m_song_handle != nullptr); }

private:
  //
  // Makes the buzzer play a note by setting the timer counter period
  // (auto-reload register) and the pulse width (compare register) to
  // generate a square wave of the desired frequency.
  //
  void set_note(uint32_t counter_period);
};
