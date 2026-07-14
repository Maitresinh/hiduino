#pragma once
#include <stdint.h>
#include <stddef.h>

namespace laserharp {

enum class LayoutMode : uint8_t { Diatonic8 = 0, Chromatic12 = 1 };
enum class GateEvent : uint8_t { None = 0, NoteOn = 1, NoteOff = 2 };

inline uint8_t beamCount(LayoutMode mode) {
  return mode == LayoutMode::Chromatic12 ? 12 : 8;
}

inline int8_t intervalFor(LayoutMode mode, uint8_t index) {
  static const int8_t diatonic[8] = {0, 2, 4, 5, 7, 9, 11, 12};
  if (mode == LayoutMode::Chromatic12) return index < 12 ? static_cast<int8_t>(index) : 0;
  return index < 8 ? diatonic[index] : 0;
}

inline uint8_t clampMidi(int note) {
  if (note < 0) return 0;
  if (note > 127) return 127;
  return static_cast<uint8_t>(note);
}

inline uint8_t noteFor(LayoutMode mode, uint8_t index, uint8_t baseNote, int8_t octaveSemitones) {
  return clampMidi(static_cast<int>(baseNote) + intervalFor(mode, index) + octaveSemitones);
}

class BeamGate {
 public:
  GateEvent update(int16_t signal, int16_t onThreshold, int16_t offThreshold,
                   uint8_t attackFrames, uint8_t releaseFrames) {
    if (!active_) {
      releaseCount_ = 0;
      if (signal >= onThreshold) {
        if (++attackCount_ >= attackFrames) {
          active_ = true;
          attackCount_ = 0;
          return GateEvent::NoteOn;
        }
      } else {
        attackCount_ = 0;
      }
    } else {
      attackCount_ = 0;
      if (signal <= offThreshold) {
        if (++releaseCount_ >= releaseFrames) {
          active_ = false;
          releaseCount_ = 0;
          return GateEvent::NoteOff;
        }
      } else {
        releaseCount_ = 0;
      }
    }
    return GateEvent::None;
  }

  bool active() const { return active_; }
  void reset() { active_ = false; attackCount_ = 0; releaseCount_ = 0; }

 private:
  bool active_ = false;
  uint8_t attackCount_ = 0;
  uint8_t releaseCount_ = 0;
};

class OctaveQuantizer {
 public:
  OctaveQuantizer(const uint16_t boundaries[3], const int8_t offsets[4], uint16_t hysteresis)
      : boundaries_(boundaries), offsets_(offsets), hysteresis_(hysteresis) {}

  int8_t update(uint16_t distanceMm) {
    while (zone_ < 3 && distanceMm > static_cast<uint16_t>(boundaries_[zone_] + hysteresis_)) ++zone_;
    while (zone_ > 0 && distanceMm + hysteresis_ < boundaries_[zone_ - 1]) --zone_;
    return offsets_[zone_];
  }

  int8_t current() const { return offsets_[zone_]; }
  void resetToBase() { zone_ = 1; }

 private:
  const uint16_t* boundaries_;
  const int8_t* offsets_;
  uint16_t hysteresis_;
  uint8_t zone_ = 1;
};

inline uint16_t evenlySpacedPosition(uint8_t index, uint8_t count, uint16_t minValue, uint16_t maxValue) {
  if (count <= 1) return static_cast<uint16_t>((static_cast<uint32_t>(minValue) + maxValue) / 2U);
  const uint32_t span = static_cast<uint32_t>(maxValue) - minValue;
  return static_cast<uint16_t>(minValue + (span * index) / (count - 1U));
}

} // namespace laserharp
