# AutoDrone - Autonomous Quadcopter Flight Controller

A hackathon project for building a stable, autonomous quadcopter using open-source hardware and PID-based attitude control.

## Overview

AutoDrone is an autonomous quadcopter flight controller built on Arduino and designed for stable hovering with position hold capabilities. The project uses a combination of IMU-based attitude estimation and PID control loops to maintain stable flight.

Originally developed as part of a larger autonomous farming robot system, AutoDrone was designed to collect aerial data about farming conditions and weed identification in orchards.

## Current Status

**Project Status:** In Development - Refactoring and Stabilization Phase

This is a hackathon project that is being cleaned up and improved for stable hovering flight. The current implementation has a working PID framework but requires tuning and bug fixes for stable operation.

## Features

### Current Features
- **3-Axis Attitude Control**: Roll, Pitch, and Yaw stabilization using PID controllers
- **IMU-based Orientation Tracking**: MPU6050 6-axis gyroscope and accelerometer
- **Remote Control Interface**: Keyboard-based ground control via serial connection
- **Real-time PID Tuning**: Adjust D-gain parameters during flight
- **Emergency Stop**: Panic button for immediate motor shutdown
- **X-configuration Motor Mixing**: Standard quadcopter motor layout

### Planned Features
- Altitude hold using ultrasonic sensor
- Autonomous hovering at set height
- GPS-based position hold
- Optical flow for drift compensation
- Waypoint navigation
- Data logging and telemetry

## Hardware Requirements

### Core Components
- **Microcontroller**: Arduino Uno (ATmega328P)
- **IMU**: MPU6050 (6-axis gyroscope + accelerometer)
- **Altitude Sensor**: HC-SR04 ultrasonic distance sensor
- **Motors**: 4x brushless DC motors with ESCs
- **Frame**: Quadcopter frame (X-configuration)
- **Power**: LiPo battery (voltage depending on motor specs)

### Wiring
See [hardware/README.md](hardware/README.md) for complete wiring diagrams and bill of materials.

| Component | Arduino Pin | Notes |
|-----------|-------------|-------|
| Motor FL (Front-Left) | Pin 3 (PWM) | ESC signal |
| Motor FR (Front-Right) | Pin 6 (PWM) | ESC signal |
| Motor BL (Back-Left) | Pin 10 (PWM) | ESC signal |
| Motor BR (Back-Right) | Pin 11 (PWM) | ESC signal |
| MPU6050 SDA | A4 | I2C Data |
| MPU6050 SCL | A5 | I2C Clock |
| HC-SR04 Trigger | Pin 4 | Digital Output |
| HC-SR04 Echo | Pin 5 | Digital Input |

## Project Structure

```
StanHacks1-AutoDrone/
├── README.md                      # This file
├── firmware/                      # Arduino firmware
│   └── flightController/
│       └── flightController.ino  # Main flight controller code
├── ground_control/                # Ground control station
│   ├── controller.py             # Keyboard-based remote control
│   └── requirements.txt          # Python dependencies
├── docs/                          # Documentation
│   ├── HARDWARE.md               # Hardware setup and wiring
│   ├── PID_TUNING.md             # PID tuning guide
│   └── IMPROVEMENTS.md           # Roadmap and improvement plans
└── hardware/                      # Hardware documentation
    └── README.md                 # Bill of materials and schematics
```

## Setup Instructions

### 1. Arduino Firmware Setup

#### Install Required Libraries
Using Arduino IDE Library Manager, install:
- `MPU6050_tockn` - IMU sensor library
- `Wire` - I2C communication (usually pre-installed)

#### Upload Firmware
1. Open `firmware/flightController/flightController.ino` in Arduino IDE
2. Select **Board**: Arduino Uno
3. Select correct **Port** for your Arduino
4. Click **Upload**

#### ESC Calibration
The firmware includes an ESC calibration routine in `setup()`:
1. Connect battery to the drone
2. ESCs will receive max throttle (254) for 1 second
3. Then min throttle (1) for 1 second
4. ESCs should beep to confirm calibration

### 2. Ground Control Setup

#### Install Python Dependencies
```bash
cd ground_control
pip install -r requirements.txt
```

#### Configure Serial Port
Edit `controller.py` line 10 if your Arduino is not on `/dev/ttyUSB0`:
```python
self.SER = serial.Serial("/dev/ttyUSB0", self.BAUD)  # Change port as needed
```

On Windows, this might be `COM3`, `COM4`, etc.

#### Run Ground Control
```bash
python controller.py
```

### 3. Pre-flight Checklist

- [ ] All motors spin freely without obstruction
- [ ] Propellers installed in correct orientation
- [ ] Battery fully charged
- [ ] ESCs calibrated
- [ ] Arduino connected via USB
- [ ] IMU sensor properly mounted (flat and centered)
- [ ] Emergency stop button ('p' key) tested
- [ ] Serial connection established (57600 baud)

## Usage

### Control Mapping

| Key | Function | Description |
|-----|----------|-------------|
| **W** | Pitch Forward | Tilt forward, move forward |
| **S** | Pitch Backward | Tilt backward, move backward |
| **A** | Roll Left | Tilt left, move left |
| **D** | Roll Right | Tilt right, move right |
| **Q** | Yaw Left | Rotate counter-clockwise |
| **E** | Yaw Right | Rotate clockwise |
| **I** | Increase Throttle | Increase altitude |
| **K** | Decrease Throttle | Decrease altitude |
| **P** | **PANIC/EMERGENCY STOP** | Immediately cut all motors |
| **1** | Increase D-gain | Increment D-gain by +1.0 |
| **2** | Decrease D-gain | Decrement D-gain by -1.0 |
| **3** | Fine-tune D-gain | Multiply D-gain by 0.75 |

### Flight Procedure

1. **Power On**: Connect battery to drone
2. **Initialize**: Arduino will calibrate ESCs and initialize IMU
3. **Start Ground Control**: Run `python controller.py`
4. **Arm**: Gradually increase throttle with 'I' key
5. **Stabilize**: Use W/A/S/D to maintain hover position
6. **Land**: Gradually decrease throttle with 'K' key
7. **Disarm**: Press 'P' for emergency stop if needed

### PID Tuning

The drone supports real-time D-gain tuning during flight. See [docs/PID_TUNING.md](docs/PID_TUNING.md) for comprehensive tuning procedures.

**Quick Start:**
- Start with D-gain = 0 (default)
- If drone oscillates, press '3' to reduce D-gain
- If drone is sluggish, press '1' to increase D-gain
- Fine-tune in small increments

## Known Issues

### Critical Bugs (Must Fix Before Flight)
1. **PID Integral Term Bug** (firmware line 60, 70, 80): Incorrect calculation - uses `+` instead of `*`
2. **PID Output Clamping Bug** (firmware line 66, 76, 86): Inverted logic on lower bound check
3. **Increment/Decrement Functions** (firmware lines 260-274): Don't work correctly (pass by value)
4. **Missing `self` Parameter** (controller.py line 17): Causes crash when sending commands
5. **Yaw Control Disabled**: All yaw PID gains set to 0
6. **Altitude Control Not Implemented**: HC-SR04 sensor configured but not used

### Safety Warnings
- Test in open area away from people and obstacles
- Start with very low throttle values
- Always have emergency stop ready ('P' key)
- Secure all wiring to prevent propeller contact
- Remove propellers for initial testing

## Contributing

This is a learning project! Contributions, suggestions, and improvements are welcome.

### Areas for Contribution
- Fix critical PID bugs
- Implement altitude hold with HC-SR04
- Add better filtering for IMU data (complementary or Kalman filter)
- Create simulation/testing framework
- Improve documentation and tutorials

## Documentation

- [Hardware Setup](hardware/README.md) - Wiring diagrams and component details
- [PID Tuning Guide](docs/PID_TUNING.md) - How to tune PID parameters
- [Improvements & Roadmap](docs/IMPROVEMENTS.md) - Planned features and TODOs

## License

Open source - feel free to use, modify, and learn from this project.

## Acknowledgments

Built during StanHacks hackathon. Thanks to the open-source community for libraries and inspiration.

---

**Warning**: This is experimental flight controller software. Always follow safety procedures and local regulations when operating drones.
