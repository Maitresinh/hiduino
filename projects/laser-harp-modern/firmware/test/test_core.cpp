#include <cassert>
#include <iostream>
#include "../include/LaserHarpCore.h"

using namespace laserharp;

static void testLayouts() {
  assert(beamCount(LayoutMode::Diatonic8) == 8);
  assert(beamCount(LayoutMode::Chromatic12) == 12);
  assert(noteFor(LayoutMode::Diatonic8, 0, 60, 0) == 60);
  assert(noteFor(LayoutMode::Diatonic8, 7, 60, 0) == 72);
  assert(noteFor(LayoutMode::Chromatic12, 11, 60, 0) == 71);
  assert(noteFor(LayoutMode::Chromatic12, 0, 60, 12) == 72);
}

static void testPositions() {
  assert(evenlySpacedPosition(0, 12, 1000, 2000) == 1000);
  assert(evenlySpacedPosition(11, 12, 1000, 2000) == 2000);
  assert(evenlySpacedPosition(0, 1, 1000, 2000) == 1500);
}

static void testGate() {
  BeamGate gate;
  assert(gate.update(90, 80, 45, 1, 2) == GateEvent::NoteOn);
  assert(gate.active());
  assert(gate.update(40, 80, 45, 1, 2) == GateEvent::None);
  assert(gate.update(40, 80, 45, 1, 2) == GateEvent::NoteOff);
  assert(!gate.active());
}

static void testOctaveHysteresis() {
  const uint16_t boundaries[3] = {300, 600, 900};
  const int8_t offsets[4] = {-12, 0, 12, 24};
  OctaveQuantizer q(boundaries, offsets, 35);
  assert(q.current() == 0);
  assert(q.update(620) == 0);
  assert(q.update(650) == 12);
  assert(q.update(580) == 12);
  assert(q.update(550) == 0);
  assert(q.update(250) == -12);
}

int main() {
  testLayouts();
  testPositions();
  testGate();
  testOctaveHysteresis();
  std::cout << "All core tests passed\n";
  return 0;
}
