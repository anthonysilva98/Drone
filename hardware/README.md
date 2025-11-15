# Hardware Setup Guide

Complete hardware specifications, wiring diagrams, and assembly instructions for the AutoDrone project.

## Bill of Materials (BOM)

### Core Electronics

| Component | Specification | Quantity | Notes |
|-----------|---------------|----------|-------|
| **Arduino Uno** | ATmega328P | 1 | Main flight controller |
| **MPU6050** | 6-axis IMU | 1 | Gyroscope + Accelerometer |
| **HC-SR04** | Ultrasonic sensor | 1 | Altitude measurement (0.02m - 4m range) |
| **ESC** | 20A-30A (depending on motors) | 4 | Electronic Speed Controllers |
| **Brushless Motors** | 1000-1400 KV | 4 | Select based on frame size |
| **Propellers** | Matching motor size | 4 | 2x CW, 2x CCW |
| **LiPo Battery** | 3S (11.1V) 2200mAh+ | 1 | 25C discharge rate minimum |
| **Power Distribution** | PDB or DIY wiring | 1 | Distribute power to ESCs |

### Frame and Mechanical

| Component | Specification | Quantity | Notes |
|-----------|---------------|----------|-------|
| **Quadcopter Frame** | 250-450mm wheelbase | 1 | X-configuration |
| **Vibration Dampers** | Rubber/silicone | 4 | For IMU mounting |
| **Standoffs/Spacers** | M3 | As needed | For stacking electronics |
| **Zip ties** | Various sizes | Multiple | Cable management |

### Connectors and Wiring

| Component | Specification | Quantity | Notes |
|-----------|---------------|----------|-------|
| **Servo Connectors** | 3-pin (ESC to Arduino) | 4 | Pre-attached to ESCs |
| **XT60 Connector** | Battery connector | 1 | Or equivalent |
| **JST Connectors** | For MPU6050 | 1 | Optional, can solder directly |
| **Wire** | 22-24 AWG | As needed | Signal wiring |
| **Power Wire** | 14-16 AWG | As needed | Battery to ESCs |
| **USB Cable** | Type B (Arduino Uno) | 1 | Programming and ground control |

### Tools Required

- Soldering iron and solder
- Wire strippers
- Screwdrivers (Phillips and flathead)
- Hex key set
- Multimeter
- Heat shrink tubing
- Hot glue gun (for securing components)

## Wiring Diagram

### Arduino Pin Assignments

```
Arduino Uno
┌─────────────────────┐
│                     │
│  Digital Pins       │
│  ┌──────────────┐   │
│  │ 0  RX        │   │ ← USB Serial (Ground Control)
│  │ 1  TX        │   │ ← USB Serial
│  │ 2            │   │
│  │ 3  PWM  ●────┼───┼─→ ESC Front-Left (FL)
│  │ 4       ●────┼───┼─→ HC-SR04 Trigger
│  │ 5       ●────┼───┼─→ HC-SR04 Echo
│  │ 6  PWM  ●────┼───┼─→ ESC Front-Right (FR)
│  │ 7            │   │
│  │ 8            │   │
│  │ 9            │   │
│  │ 10 PWM  ●────┼───┼─→ ESC Back-Left (BL)
│  │ 11 PWM  ●────┼───┼─→ ESC Back-Right (BR)
│  │ 12           │   │
│  │ 13           │   │
│  └──────────────┘   │
│                     │
│  Analog Pins        │
│  ┌──────────────┐   │
│  │ A0           │   │
│  │ A1           │   │
│  │ A2           │   │
│  │ A3           │   │
│  │ A4 (SDA) ●───┼───┼─→ MPU6050 SDA
│  │ A5 (SCL) ●───┼───┼─→ MPU6050 SCL
│  └──────────────┘   │
│                     │
│  Power              │
│  ┌──────────────┐   │
│  │ 5V      ●────┼───┼─→ MPU6050 VCC, HC-SR04 VCC
│  │ GND     ●────┼───┼─→ MPU6050 GND, HC-SR04 GND
│  │ VIN          │   │
│  └──────────────┘   │
└─────────────────────┘
```

### Motor Layout (X-Configuration)

```
        FRONT
         ↑

    FL      FR
      ╲   ╱
       ╲ ╱
        X
       ╱ ╲
      ╱   ╲
    BL      BR

    FL = Front-Left  (Pin 3)  - CW rotation
    FR = Front-Right (Pin 6)  - CCW rotation
    BL = Back-Left   (Pin 10) - CCW rotation
    BR = Back-Right  (Pin 11) - CW rotation
```

### Power Distribution

```
┌─────────────┐
│  LiPo       │
│  Battery    │
│  11.1V (3S) │
└──────┬──────┘
       │
       │ XT60 Connector
       │
┌──────┴──────────────────────┐
│  Power Distribution Board   │
│  (or wiring harness)        │
└──┬────┬────┬────┬───────────┘
   │    │    │    │
   │    │    │    │
  ESC  ESC  ESC  ESC
  FL   FR   BL   BR
   │    │    │    │
   ▼    ▼    ▼    ▼
 Motor Motor Motor Motor

Note: ESC BECs can power Arduino via 5V rail
      Or use separate 5V regulator
```

### MPU6050 Wiring

```
MPU6050 Module
┌────────────┐
│    VCC  ●──┼─→ Arduino 5V
│    GND  ●──┼─→ Arduino GND
│    SCL  ●──┼─→ Arduino A5 (SCL)
│    SDA  ●──┼─→ Arduino A4 (SDA)
│    XDA     │   (not used)
│    XCL     │   (not used)
│    AD0     │   (leave floating or GND)
│    INT     │   (not used)
└────────────┘
```

### HC-SR04 Wiring

```
HC-SR04 Module
┌────────────┐
│   VCC   ●──┼─→ Arduino 5V
│   Trig  ●──┼─→ Arduino Pin 4
│   Echo  ●──┼─→ Arduino Pin 5
│   GND   ●──┼─→ Arduino GND
└────────────┘

Note: Mount facing downward for altitude measurement
```

### ESC Connections

Each ESC has three connections:

```
ESC (×4)
├─ Signal Wire (white/yellow) ─→ Arduino PWM Pin (3, 6, 10, or 11)
├─ +5V Wire (red)             ─→ Can connect to Arduino 5V rail (optional)
└─ GND Wire (black/brown)     ─→ Arduino GND (REQUIRED for common ground)

ESC to Motor:
└─ 3 motor wires ─→ Connect to brushless motor phases
                    (swap any two wires to reverse direction)
```

## Assembly Instructions

### 1. Frame Assembly
1. Assemble the quadcopter frame according to manufacturer instructions
2. Install motors on the four arms
3. Ensure motor rotation directions match the X-configuration diagram
4. Install propellers (CW on FL/BR, CCW on FR/BL)

### 2. ESC Installation
1. Mount ESCs near motors (one per arm)
2. Connect ESC power wires to power distribution board
3. Connect ESC signal wires to Arduino pins:
   - FL → Pin 3
   - FR → Pin 6
   - BL → Pin 10
   - BR → Pin 11
4. **Connect all ESC grounds to Arduino GND** (critical for signal reference)

### 3. Arduino Mounting
1. Mount Arduino on center plate using vibration dampers or standoffs
2. Ensure clearance for USB cable connection
3. Route ESC signal wires to Arduino PWM pins
4. Secure wiring with zip ties

### 4. Sensor Installation

#### MPU6050 (IMU)
1. Mount as close to center of gravity as possible
2. Use vibration dampening (foam tape or rubber standoffs)
3. Ensure sensor is level and aligned with frame axes
4. Wire to Arduino I2C pins (A4/SDA, A5/SCL)
5. Connect VCC (5V) and GND

#### HC-SR04 (Ultrasonic)
1. Mount on bottom of drone, facing downward
2. Ensure clear line of sight to ground (no propeller obstruction)
3. Wire Trig to Pin 4, Echo to Pin 5
4. Connect VCC (5V) and GND

### 5. Power System
1. Install power distribution board on center plate
2. Solder battery connector (XT60) to PDB
3. Connect all four ESCs to PDB
4. Add capacitor across PDB power rails (100-220µF, optional but recommended)
5. Consider adding a power switch between battery and PDB

### 6. Final Assembly
1. Double-check all connections against wiring diagram
2. Secure all wiring with zip ties
3. Ensure no wires near propellers
4. Apply hot glue to prevent connectors from vibrating loose
5. Balance the drone (center of gravity should be at geometric center)

## Testing and Verification

### Power-On Tests (NO PROPELLERS!)

1. **Visual Inspection**
   - [ ] No short circuits
   - [ ] All connections secure
   - [ ] No loose wires

2. **Multimeter Tests**
   - [ ] Battery voltage correct (11.1V for 3S)
   - [ ] 5V rail reading 4.8-5.2V
   - [ ] No shorts between power and ground

3. **Arduino Tests**
   - [ ] Upload test sketch
   - [ ] Serial communication working
   - [ ] LED blinking confirms operation

4. **Sensor Tests**
   - [ ] MPU6050 I2C communication (address 0x68)
   - [ ] MPU6050 reading valid angles
   - [ ] HC-SR04 reading distances

5. **Motor Tests (NO PROPS!)**
   - [ ] All motors spin in correct direction
   - [ ] ESCs respond to throttle commands
   - [ ] No unusual vibrations or noises
   - [ ] Emergency stop works

### ESC Calibration Procedure

1. Remove all propellers
2. Upload flight controller code
3. Connect battery
4. ESCs will automatically calibrate (max → min throttle)
5. Listen for confirmation beeps from ESCs
6. If calibration fails, manually calibrate:
   - Set throttle to maximum in code
   - Power on ESCs
   - Wait for beeps
   - Set throttle to minimum
   - Wait for beeps

## Safety Considerations

### Pre-Flight
- Remove propellers for all software testing
- Test in open area, minimum 10m radius clear
- Ensure battery is secure and won't shift during flight
- Check all screws and fasteners are tight
- Verify emergency stop functionality

### During Operation
- Never approach powered drone
- Always have clear line of sight
- Monitor battery voltage (land at 3.5V per cell minimum)
- Keep USB cable management clear of propellers
- Be ready to cut power immediately

### Troubleshooting

| Issue | Possible Cause | Solution |
|-------|----------------|----------|
| Motors won't arm | ESCs not calibrated | Run ESC calibration routine |
| One motor not responding | Loose connection | Check signal and ground wires |
| IMU reading incorrect | Not calibrated | Place flat, recalibrate on startup |
| Drone drifts heavily | IMU misaligned | Ensure IMU is level and centered |
| Brown-outs/resets | Voltage drop | Add capacitor, check battery |
| Oscillations | PID tuning | See PID_TUNING.md |

## Recommended Upgrades

### Short Term
- Add LED indicators for status
- Voltage monitoring via analog pin
- Buzzer for low battery warning

### Long Term
- GPS module for position hold
- Optical flow sensor for indoor flight
- FPV camera and transmitter
- Telemetry radio (replace USB)
- Higher precision IMU (e.g., BNO055)

## Specifications Summary

| Parameter | Value |
|-----------|-------|
| **Weight** | ~500-800g (depending on components) |
| **Flight Time** | 5-10 minutes (with 2200mAh battery) |
| **Control Range** | USB cable length (wired) |
| **Max Altitude (HC-SR04)** | 4 meters |
| **Update Rate** | ~50-100Hz (Arduino loop speed) |
| **Response Time** | <50ms |

---

**Always follow local regulations and safety guidelines when building and operating drones.**
