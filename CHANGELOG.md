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

- **CRITICAL**: Fixed PID output clamping logic error in `firmware/flightController/flightController.ino` (lines 66, 76, 86)
  - Changed `else if (pidOutputRoll < pidMaxRoll)` to `else if (pidOutputRoll < -pidMaxRoll)`
  - Changed `else if (pidOutputPitch < pidMaxPitch)` to `else if (pidOutputPitch < -pidMaxPitch)`
  - Changed `else if (pidOutputYaw < pidMaxYaw)` to `else if (pidOutputYaw < -pidMaxYaw)`
  - Impact: Lower bound clamping now correctly triggers at negative max values instead of always triggering for any negative value
  - This prevents small negative PID corrections from being incorrectly amplified to maximum negative values, which would cause severe instability
