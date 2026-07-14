#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "config.h"
#include "LaserHarpCore.h"

#if ENABLE_HEIGHT_SENSOR
#include <VL53L4CD.h>
#endif

using laserharp::BeamGate;
using laserharp::GateEvent;
using laserharp::LayoutMode;

namespace {

enum class RunState : uint8_t { SafeOff, Armed, Fault };

struct BeamRuntime {
  BeamGate gate;
  int16_t baseline = 0;
  int16_t lastDelta = 0;
  uint8_t activeNote = 0;
};

BeamRuntime beams[12];
RunState runState = RunState::SafeOff;
LayoutMode layout = LayoutMode::Diatonic8;
uint16_t warmupFramesRemaining = cfg::BASELINE_WARMUP_FRAMES;
uint32_t frameCounter = 0;
uint32_t lastDiagnosticMs = 0;
int8_t currentOctaveSemitones = 0;
#if ENABLE_HEIGHT_SENSOR
uint32_t lastHeightValidMs = 0;
#endif
bool previousArmPressed = false;
bool previousLayout12 = false;
uint32_t armChangeMs = 0;
uint32_t layoutChangeMs = 0;
SPISettings dacSpi(cfg::DAC_SPI_HZ, MSBFIRST, SPI_MODE0);

laserharp::OctaveQuantizer octaveQuantizer(
    cfg::HEIGHT_BOUNDARIES_MM,
    cfg::HEIGHT_OCTAVE_SEMITONES,
    cfg::HEIGHT_HYSTERESIS_MM);

#if ENABLE_HEIGHT_SENSOR
VL53L4CD heightSensor;
bool heightSensorAvailable = false;
#endif

inline void laserOff() {
  digitalWriteFast(cfg::PIN_LASER_TTL, cfg::LASER_ACTIVE_HIGH ? LOW : HIGH);
}

inline void laserOn() {
  if (runState == RunState::Armed && digitalReadFast(cfg::PIN_INTERLOCK_MON) == HIGH) {
    digitalWriteFast(cfg::PIN_LASER_TTL, cfg::LASER_ACTIVE_HIGH ? HIGH : LOW);
  }
}

inline bool interlockClosed() {
  return digitalReadFast(cfg::PIN_INTERLOCK_MON) == HIGH;
}

void writeDacA(uint16_t value) {
  if (value > 4095) value = 4095;
  const uint16_t command = static_cast<uint16_t>(0x3000U | value);
  SPI.beginTransaction(dacSpi);
  digitalWriteFast(cfg::PIN_DAC_CS, LOW);
  SPI.transfer16(command);
  digitalWriteFast(cfg::PIN_DAC_CS, HIGH);
  SPI.endTransaction();
}

uint16_t readPhotoAverage(uint8_t samples) {
  uint32_t total = 0;
  for (uint8_t i = 0; i < samples; ++i) total += analogRead(cfg::PIN_PHOTO);
  return static_cast<uint16_t>(total / samples);
}

int16_t measureReflection() {
  laserOff();
  const uint16_t ambient = readPhotoAverage(cfg::ADC_SAMPLES_AMBIENT);
  laserOn();
  delayMicroseconds(cfg::LASER_RISE_US);
  const uint16_t lit = readPhotoAverage(cfg::ADC_SAMPLES_LIT);
  if (cfg::VISIBLE_DWELL_US > 0) delayMicroseconds(cfg::VISIBLE_DWELL_US);
  laserOff();
  return static_cast<int16_t>(static_cast<int32_t>(lit) - static_cast<int32_t>(ambient));
}

void sendNoteOn(uint8_t note) {
  usbMIDI.sendNoteOn(note, cfg::MIDI_VELOCITY, cfg::MIDI_CHANNEL);
  usbMIDI.send_now();
}

void sendNoteOff(uint8_t note) {
  usbMIDI.sendNoteOff(note, 0, cfg::MIDI_CHANNEL);
  usbMIDI.send_now();
}

void allNotesOff() {
  for (auto &beam : beams) {
    if (beam.gate.active()) sendNoteOff(beam.activeNote);
    beam.gate.reset();
    beam.activeNote = 0;
  }
  usbMIDI.sendControlChange(123, 0, cfg::MIDI_CHANNEL);
  usbMIDI.send_now();
}

void resetDetection() {
  allNotesOff();
  for (auto &beam : beams) {
    beam.baseline = 0;
    beam.lastDelta = 0;
  }
  warmupFramesRemaining = cfg::BASELINE_WARMUP_FRAMES;
  frameCounter = 0;
}

void enterSafeOff() {
  laserOff();
  writeDacA(cfg::DAC_CENTER);
  if (runState == RunState::Armed) allNotesOff();
  runState = RunState::SafeOff;
  digitalWriteFast(cfg::PIN_STATUS_LED, LOW);
}

void enterFault() {
  laserOff();
  writeDacA(cfg::DAC_CENTER);
  allNotesOff();
  runState = RunState::Fault;
  digitalWriteFast(cfg::PIN_STATUS_LED, HIGH);
}

void armIfAllowed() {
  if (!interlockClosed()) {
    enterFault();
    return;
  }
  resetDetection();
  runState = RunState::Armed;
  digitalWriteFast(cfg::PIN_STATUS_LED, HIGH);
}

void serviceArmButton() {
  const bool pressed = digitalReadFast(cfg::PIN_ARM) == LOW;
  const uint32_t now = millis();
  if (pressed != previousArmPressed && now - armChangeMs >= cfg::CONTROL_DEBOUNCE_MS) {
    armChangeMs = now;
    previousArmPressed = pressed;
    if (pressed) {
      if (runState == RunState::Armed) enterSafeOff();
      else armIfAllowed();
    }
  }
}

void applyLayout(LayoutMode newLayout) {
  if (newLayout == layout) return;
  laserOff();
  writeDacA(cfg::DAC_CENTER);
  layout = newLayout;
  resetDetection();
}

void serviceLayoutSwitch() {
  const bool mode12 = digitalReadFast(cfg::PIN_LAYOUT_12) == LOW;
  const uint32_t now = millis();
  if (mode12 != previousLayout12 && now - layoutChangeMs >= cfg::CONTROL_DEBOUNCE_MS) {
    layoutChangeMs = now;
    previousLayout12 = mode12;
    applyLayout(mode12 ? LayoutMode::Chromatic12 : LayoutMode::Diatonic8);
  }
}

#if ENABLE_HEIGHT_SENSOR
void setupHeightSensor() {
  Wire.begin();
  Wire.setClock(400000);
  heightSensor.setBus(&Wire);
  heightSensor.setTimeout(0);
  heightSensorAvailable = heightSensor.init(true, false);
  if (heightSensorAvailable) {
    heightSensor.setRangeTiming(10, 0);
    heightSensor.startContinuous();
    lastHeightValidMs = millis();
  }
}

void retuneHeldNotesIfEnabled(int8_t oldOctave, int8_t newOctave) {
  if (!cfg::HEIGHT_RETUNE_HELD_NOTES || oldOctave == newOctave) return;
  const uint8_t count = laserharp::beamCount(layout);
  for (uint8_t i = 0; i < count; ++i) {
    if (!beams[i].gate.active()) continue;
    sendNoteOff(beams[i].activeNote);
    beams[i].activeNote = laserharp::noteFor(layout, i, cfg::BASE_NOTE, newOctave);
    sendNoteOn(beams[i].activeNote);
  }
}

void serviceHeightSensor() {
  if (!heightSensorAvailable) return;
  if (heightSensor.dataReady()) {
    const uint16_t distance = heightSensor.read(false);
    const bool valid = heightSensor.ranging_data.range_status == 0 &&
                       distance >= cfg::HEIGHT_MIN_MM && distance <= cfg::HEIGHT_MAX_MM;
    if (valid) {
      const int8_t oldOctave = currentOctaveSemitones;
      currentOctaveSemitones = octaveQuantizer.update(distance);
      lastHeightValidMs = millis();
      retuneHeldNotesIfEnabled(oldOctave, currentOctaveSemitones);
    }
  }
  if (millis() - lastHeightValidMs > cfg::HEIGHT_TIMEOUT_MS) {
    const int8_t oldOctave = currentOctaveSemitones;
    octaveQuantizer.resetToBase();
    currentOctaveSemitones = 0;
    lastHeightValidMs = millis();
    retuneHeldNotesIfEnabled(oldOctave, currentOctaveSemitones);
  }
}
#else
void setupHeightSensor() {}
void serviceHeightSensor() {}
#endif

void processBeam(uint8_t index, uint8_t count) {
  const uint16_t position = laserharp::evenlySpacedPosition(index, count, cfg::DAC_MIN, cfg::DAC_MAX);
  laserOff();
  writeDacA(position);
  delayMicroseconds(cfg::GALVO_SETTLE_US);

  const int16_t rawSignal = measureReflection();
  BeamRuntime &beam = beams[index];

  if (frameCounter == 0) {
    beam.baseline = rawSignal;
  } else if (warmupFramesRemaining > 0 || !beam.gate.active()) {
    const int16_t error = static_cast<int16_t>(rawSignal - beam.baseline);
    if (warmupFramesRemaining > 0) {
      beam.baseline = static_cast<int16_t>(beam.baseline + error / (1 << cfg::BASELINE_WARMUP_SHIFT));
    } else if (error < cfg::PER_BEAM_ON_DELTA[index]) {
      beam.baseline = static_cast<int16_t>(beam.baseline + error / (1 << cfg::BASELINE_EMA_SHIFT));
    }
  }

  const int16_t delta = static_cast<int16_t>(rawSignal - beam.baseline);
  beam.lastDelta = delta;
  if (warmupFramesRemaining > 0) return;

  const GateEvent event = beam.gate.update(
      delta,
      cfg::PER_BEAM_ON_DELTA[index],
      cfg::PER_BEAM_OFF_DELTA[index],
      cfg::ATTACK_CONFIRM_FRAMES,
      cfg::RELEASE_CONFIRM_FRAMES);

  if (event == GateEvent::NoteOn) {
    beam.activeNote = laserharp::noteFor(layout, index, cfg::BASE_NOTE, currentOctaveSemitones);
    sendNoteOn(beam.activeNote);
  } else if (event == GateEvent::NoteOff) {
    sendNoteOff(beam.activeNote);
    beam.activeNote = 0;
  }
}

void scanFrame() {
  const uint8_t count = laserharp::beamCount(layout);
  for (uint8_t i = 0; i < count; ++i) {
    if (!interlockClosed()) {
      enterFault();
      return;
    }
    processBeam(i, count);
  }
  ++frameCounter;
  if (warmupFramesRemaining > 0) --warmupFramesRemaining;
}

void serviceDiagnostics() {
  const uint32_t now = millis();
  if (now - lastDiagnosticMs < cfg::DIAGNOSTIC_INTERVAL_MS) return;
  lastDiagnosticMs = now;
  if (Serial && Serial.availableForWrite() > 96) {
    Serial.printf("state=%u mode=%u beams=%u octave=%d frames=%lu warmup=%u\n",
                  static_cast<unsigned>(runState),
                  static_cast<unsigned>(layout),
                  laserharp::beamCount(layout),
                  currentOctaveSemitones,
                  static_cast<unsigned long>(frameCounter),
                  warmupFramesRemaining);
  }
}

} // namespace

void setup() {
  pinMode(cfg::PIN_LAYOUT_12, INPUT_PULLUP);
  pinMode(cfg::PIN_ARM, INPUT_PULLUP);
  pinMode(cfg::PIN_INTERLOCK_MON, INPUT);
  pinMode(cfg::PIN_LASER_TTL, OUTPUT);
  pinMode(cfg::PIN_STATUS_LED, OUTPUT);
  pinMode(cfg::PIN_DAC_CS, OUTPUT);

  laserOff();
  digitalWriteFast(cfg::PIN_STATUS_LED, LOW);
  digitalWriteFast(cfg::PIN_DAC_CS, HIGH);

  analogReadResolution(12);
  analogReadAveraging(1);
  SPI.begin();
  writeDacA(cfg::DAC_CENTER);

  Serial.begin(115200);
  setupHeightSensor();

  previousLayout12 = digitalReadFast(cfg::PIN_LAYOUT_12) == LOW;
  layout = previousLayout12 ? LayoutMode::Chromatic12 : LayoutMode::Diatonic8;

  if (!interlockClosed()) enterFault();
  else enterSafeOff();
}

void loop() {
  while (usbMIDI.read()) {}
  serviceArmButton();
  serviceLayoutSwitch();
  if (!interlockClosed() && runState != RunState::Fault) enterFault();
  if (runState == RunState::Armed) scanFrame();
  else laserOff();
  serviceHeightSensor();
  serviceDiagnostics();
}
