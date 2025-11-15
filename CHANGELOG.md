# Changelog

All notable changes to the AutoDrone project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).

## [Unreleased]

### Planned
- Fix critical PID calculation bugs
- Implement altitude hold using HC-SR04
- Enable and tune yaw control
- Add comprehensive telemetry logging
- Implement failsafe mechanisms
- Add pre-flight safety checks

---

## [2.0.0] - 2025-11-15 - Project Restructure

### Added
- **New Documentation**
  - Comprehensive README.md with setup instructions and usage guide
  - Hardware setup guide (hardware/README.md) with wiring diagrams and BOM
  - PID tuning guide (docs/PID_TUNING.md) with multiple tuning methods
  - Improvements roadmap (docs/IMPROVEMENTS.md) with prioritized TODOs
  - This CHANGELOG to track project history

- **New Project Structure**
  - Created `firmware/` directory for Arduino code
  - Created `ground_control/` directory for Python controller
  - Created `docs/` directory for documentation
  - Created `hardware/` directory for hardware documentation
  - Added `requirements.txt` for Python dependencies

- **Documentation Features**
  - Complete control mapping reference
  - Pre-flight checklist
  - Safety warnings and procedures
  - Troubleshooting guide
  - Bill of materials (BOM)
  - Wiring diagrams (ASCII art)
  - Motor layout diagram
  - PID theory explanations
  - Multiple tuning methods (Ziegler-Nichols, Manual, Recommended values)
  - Common problem solutions
  - Advanced tuning techniques

### Changed
- Reorganized repository structure for better maintainability
- Updated README with current project status
- Moved `flightController.ino` to `firmware/flightController/`
- Moved `controller.py` to `ground_control/`

### Documented (Not Yet Fixed)
- **Critical Bugs Identified**:
  - BUG-001: PID integral term uses `+` instead of `*`
  - BUG-002: PID output clamping inverted logic on lower bound
  - BUG-003: Increment/Decrement functions pass by value (don't work)
  - BUG-004: Missing `self` parameter in Python `_sendCommand()`
  - BUG-005: Variable name typo `pidIGainPitchaw`
  - BUG-006: Inconsistent `thrust` vs `throttle` variable usage

### Notes
- This release focuses on documentation and project organization
- No code functionality changes yet
- Bug fixes and new features planned for next release
- Original hackathon code preserved in git history

---

## [1.0.0] - 2019-07-07 - Hackathon Version

### Added
- Python remote control interface (controller.py)
  - Tkinter-based GUI for keyboard input
  - Serial communication at 57600 baud
  - Key mapping for flight controls
  - Real-time PID tuning keys (1, 2, 3)

### Changed
- Updated from previous development iterations

---

## [0.3.0] - 2019-06-25 - PID Control Implementation

### Added
- **PID Control System**
  - Roll PID controller (P=1.0, I=0.04, D=0.0)
  - Pitch PID controller (matches Roll gains)
  - Yaw PID controller (disabled - all gains = 0)
  - PID output clamping (±2000)
  - Integral term anti-windup

- **Real-time Tuning**
  - D-gain adjustment via keyboard during flight
  - Key '1': Increment D-gain by +1.0
  - Key '2': Decrement D-gain by -1.0
  - Key '3': Multiply D-gain by 0.75

### Changed
- Major refactor: +106 insertions
- Integrated PID calculations into main loop
- Updated motor mixing to use PID outputs

### Known Issues
- PID not yet tuned for stable flight
- Yaw control not implemented
- HC-SR04 sensor defined but not used
- Several calculation bugs (documented in v2.0.0)

---

## [0.2.0] - 2019-06-25 - Remote Control

### Added
- Serial communication interface
- Keyboard-based remote control commands
- Manual throttle control (I/K keys)
- Pitch control (W/S keys)
- Roll control (A/D keys)
- Yaw control (Q/E keys)
- Emergency stop (P key)
- Dead-band for control inputs (±8 around 1500)

### Changed
- Improved input handling
- Added command parsing via switch statements

---

## [0.1.0] - 2019-06-15 - Initial Implementation

### Added
- **Core Functionality**
  - Arduino Uno-based flight controller
  - MPU6050 IMU integration
  - 4-motor ESC control (PWM on pins 3, 6, 10, 11)
  - X-configuration motor mixing
  - ESC initialization sequence

- **Sensor Support**
  - MPU6050 via I2C (SDA/SCL on A4/A5)
  - HC-SR04 ultrasonic sensor (pins 4, 5) - configured but not used
  - Gyroscope and accelerometer fusion (99.96% gyro, 0.04% accel)

- **Safety Features**
  - ESC calibration on startup
  - Emergency stop function
  - Motor output limits

### Hardware
- **Microcontroller**: Arduino Uno (ATmega328P)
- **IMU**: MPU6050 (6-axis gyro + accel)
- **Distance Sensor**: HC-SR04 ultrasonic
- **Motors**: 4× brushless DC with ESCs
- **Configuration**: Quadcopter X-frame

### Libraries
- `MPU6050_tockn` - IMU sensor interface
- `Wire` - I2C communication
- `Keyboard` - Input handling

---

## Version History Summary

| Version | Date | Description |
|---------|------|-------------|
| 2.0.0 | 2025-11-15 | Project restructure and documentation |
| 1.0.0 | 2019-07-07 | Python remote control added |
| 0.3.0 | 2019-06-25 | PID control implementation |
| 0.2.0 | 2019-06-25 | Remote control commands |
| 0.1.0 | 2019-06-15 | Initial flight controller |

---

## Future Releases (Planned)

### [2.1.0] - Critical Bug Fixes
- Fix all identified PID calculation bugs
- Fix Python controller bugs
- Verify basic functionality
- Test without flight

### [2.2.0] - Stable Hovering
- Implement altitude hold with HC-SR04
- Enable and tune yaw control
- Improve IMU filtering
- Add control deadbands
- First stable hover test

### [3.0.0] - Safety & Quality
- Pre-flight checks
- Failsafe mechanisms
- Comprehensive logging
- Code modularization
- Telemetry downlink

### [4.0.0] - Advanced Features
- Flight modes (Manual, Stabilize, Altitude Hold)
- Wireless control (Bluetooth/WiFi)
- GPS integration
- Position hold
- Waypoint navigation

---

## Development Timeline

```
2019-06 : Initial hackathon development
2019-07 : Last updates during hackathon
...
2025-11 : Project revival and restructure
```

**Gap**: ~6 years between hackathon and current restructure
**Reason**: Learning more about PIDs and localization systems
**Goal**: Achieve stable hovering flight

---

## Acknowledgments

### Original Hackathon Team
- Developed during StanHacks hackathon
- Built in June-July 2019
- Focus: Autonomous farming drone for weed detection

### Current Development
- Project cleanup and restructure
- Improved PID understanding
- Better localization system knowledge
- Goal: Stable hovering as foundation for future features

### Open Source Libraries
- **MPU6050_tockn** - IMU library
- **Arduino** - Development platform
- **Python pyserial** - Serial communication

---

**Note**: This changelog will be updated as development progresses. All dates prior to 2025-11-15 are reconstructed from git commit history.
