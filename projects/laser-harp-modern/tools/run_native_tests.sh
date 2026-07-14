#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="${TMPDIR:-/tmp}/laser_harp_core_tests"
g++ -std=c++17 -Wall -Wextra -Werror \
  -I"$ROOT/firmware/include" \
  "$ROOT/firmware/test/test_core.cpp" \
  -o "$OUT"
"$OUT"
