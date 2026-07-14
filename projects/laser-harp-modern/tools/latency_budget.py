#!/usr/bin/env python3
import argparse

p = argparse.ArgumentParser(description="Estimate one Laser Harp scan frame")
p.add_argument('--beams', type=int, default=12)
p.add_argument('--settle-us', type=float, default=80)
p.add_argument('--laser-rise-us', type=float, default=12)
p.add_argument('--visible-dwell-us', type=float, default=20)
p.add_argument('--adc-read-us', type=float, default=3.0)
p.add_argument('--adc-samples-off', type=int, default=3)
p.add_argument('--adc-samples-on', type=int, default=3)
p.add_argument('--dac-us', type=float, default=3.0)
a = p.parse_args()
slot = (a.settle_us + a.laser_rise_us + a.visible_dwell_us +
        a.adc_read_us * (a.adc_samples_off + a.adc_samples_on) + a.dac_us)
frame = slot * a.beams
print(f"slot:  {slot:.1f} us")
print(f"frame: {frame:.1f} us ({frame/1000:.3f} ms)")
print(f"scan rate: {1_000_000/frame:.1f} Hz")
print(f"worst detection wait before USB enqueue: about {frame/1000:.3f} ms")
