# Changelog

All notable changes to the AutoDrone project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Fixed
- **CRITICAL**: Fixed PID integral term calculation error in `firmware/flightController/flightController.ino` (lines 60, 70, 80)
  - Changed `pidIMemRoll += pidIGainRoll + pidErrTemp` to `pidIMemRoll += pidIGainRoll * pidErrTemp`
  - Changed `pidIMemPitch += pidIGainPitch + pidErrTemp` to `pidIMemPitch += pidIGainPitch * pidErrTemp`
  - Changed `pidIMemYaw += pidIGainYaw + pidErrTemp` to `pidIMemYaw += pidIGainYaw * pidErrTemp`
  - Impact: Integral term now correctly accumulates error scaled by gain instead of adding error to gain
  - This fixes unpredictable PID behavior that would cause dangerous flight instability
