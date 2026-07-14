#pragma once
#include <Arduino.h>

#ifndef ENABLE_HEIGHT_SENSOR
#define ENABLE_HEIGHT_SENSOR 1
#endif

namespace cfg {

constexpr uint8_t PIN_LAYOUT_12 = 2;
constexpr uint8_t PIN_ARM = 3;
constexpr uint8_t PIN_INTERLOCK_MON = 4;
constexpr uint8_t PIN_LASER_TTL = 6;
constexpr uint8_t PIN_STATUS_LED = 7;
constexpr uint8_t PIN_DAC_CS = 10;
constexpr uint8_t PIN_PHOTO = A1;

constexpr bool LASER_ACTIVE_HIGH = true;
constexpr uint8_t MIDI_CHANNEL = 1;
constexpr uint8_t BASE_NOTE = 60;
constexpr uint8_t MIDI_VELOCITY = 100;

constexpr uint16_t DAC_CENTER = 2048;
constexpr uint16_t DAC_MIN = 1848;
constexpr uint16_t DAC_MAX = 2248;
constexpr uint32_t DAC_SPI_HZ = 20000000;

constexpr uint16_t GALVO_SETTLE_US = 80;
constexpr uint16_t LASER_RISE_US = 12;
constexpr uint16_t VISIBLE_DWELL_US = 20;
constexpr uint8_t ADC_SAMPLES_AMBIENT = 3;
constexpr uint8_t ADC_SAMPLES_LIT = 3;

constexpr int16_t PHOTO_ON_DELTA = 80;
constexpr int16_t PHOTO_OFF_DELTA = 45;
constexpr uint8_t ATTACK_CONFIRM_FRAMES = 1;
constexpr uint8_t RELEASE_CONFIRM_FRAMES = 2;
constexpr uint8_t BASELINE_EMA_SHIFT = 5;
constexpr uint8_t BASELINE_WARMUP_SHIFT = 2;
constexpr uint16_t BASELINE_WARMUP_FRAMES = 48;

constexpr int16_t PER_BEAM_ON_DELTA[12] = {80,80,80,80,80,80,80,80,80,80,80,80};
constexpr int16_t PER_BEAM_OFF_DELTA[12] = {45,45,45,45,45,45,45,45,45,45,45,45};

constexpr uint16_t HEIGHT_MIN_MM = 80;
constexpr uint16_t HEIGHT_MAX_MM = 1100;
constexpr uint16_t HEIGHT_BOUNDARIES_MM[3] = {300, 600, 900};
constexpr int8_t HEIGHT_OCTAVE_SEMITONES[4] = {-12, 0, 12, 24};
constexpr uint16_t HEIGHT_HYSTERESIS_MM = 35;
constexpr uint32_t HEIGHT_TIMEOUT_MS = 600;
constexpr bool HEIGHT_RETUNE_HELD_NOTES = false;

constexpr uint16_t CONTROL_DEBOUNCE_MS = 25;
constexpr uint32_t DIAGNOSTIC_INTERVAL_MS = 2000;

} // namespace cfg
