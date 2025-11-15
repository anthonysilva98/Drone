# Improvements & Roadmap

Comprehensive list of improvements, bug fixes, and features needed to achieve stable hovering flight.

## Priority Levels
- 🔴 **CRITICAL** - Must fix before flight (safety/functionality)
- 🟡 **HIGH** - Needed for stable hovering
- 🟢 **MEDIUM** - Quality of life improvements
- 🔵 **LOW** - Nice-to-have features

---

## Phase 1: Critical Bug Fixes 🔴

### BUG-002: PID Output Clamping Logic Error
**Priority**: 🔴 CRITICAL
**File**: `firmware/flightController/flightController.ino`
**Lines**: 66, 76, 86

**Issue**:
```cpp
// CURRENT (WRONG):
else if (pidOutputRoll < pidMaxRoll) pidOutputRoll = pidMaxRoll * -1;

// SHOULD BE:
else if (pidOutputRoll < -pidMaxRoll) pidOutputRoll = -pidMaxRoll;
```

**Impact**: Lower bound clamping never triggers, allows extreme negative values

**Fix Difficulty**: Easy (3 line changes)

---

### BUG-003: Increment/Decrement Functions Don't Work
**Priority**: 🟡 HIGH
**File**: `firmware/flightController/flightController.ino`
**Lines**: 260-274

**Issue**: Functions pass by value, not reference - changes don't persist

```cpp
// CURRENT (WRONG):
void Increment(int valToIncrement) {
  if(valToIncrement > 2000)
    valToIncrement = 2000;
  else
    valToIncrement++;
}

// SHOULD BE:
void Increment(int& valToIncrement) {
  if(valToIncrement >= 2000)
    valToIncrement = 2000;
  else
    valToIncrement++;
}
```

**Impact**: Throttle and control inputs don't respond to keyboard

**Fix Difficulty**: Easy (add reference parameters)

---

### BUG-004: Missing `self` Parameter in Python
**Priority**: 🔴 CRITICAL
**File**: `ground_control/controller.py`
**Line**: 17

**Issue**:
```python
# CURRENT (WRONG):
def _sendCommand(command):
    self.SER.write(command)

# SHOULD BE:
def _sendCommand(self, command):
    self.SER.write(command)
```

**Impact**: Crashes when trying to send commands

**Fix Difficulty**: Easy (1 line change)

---

### BUG-005: Variable Name Typo
**Priority**: 🟢 MEDIUM
**File**: `firmware/flightController/flightController.ino`
**Line**: 41

**Issue**:
```cpp
// CURRENT:
float pidIGainPitchaw = 0.;

// SHOULD BE:
float pidIGainYaw = 0.;
```

**Impact**: Yaw I-gain not accessible (currently disabled anyway)

**Fix Difficulty**: Easy (1 line change)

---

### BUG-006: Inconsistent Variable Usage
**Priority**: 🟡 HIGH
**File**: `firmware/flightController/flightController.ino`
**Lines**: 146, 151

**Issue**: Inconsistent use of `thrust` vs `throttle`
- Line 146: `Increment(thrust);`
- Line 151: `Decrement(throttle);`

**Fix**: Use `throttle` consistently

---

## Phase 2: Hovering Stability 🟡

### FEATURE-001: Implement Altitude Hold
**Priority**: 🟡 HIGH
**Estimated Time**: 4-6 hours

**Requirements**:
1. Read HC-SR04 distance sensor in main loop
2. Implement altitude PID controller
3. Add altitude setpoint control
4. Integrate with throttle output

**Implementation Steps**:
```cpp
// 1. Add altitude reading function
float readAltitude() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = duration * 0.034 / 2;  // cm
  return distance;
}

// 2. Add altitude PID
float altitudeP = 2.0;
float altitudeI = 0.1;
float altitudeD = 1.0;
float targetAltitude = 50.0;  // cm

// 3. Calculate altitude correction
float calculateAltitudePID() {
  float currentAlt = readAltitude();
  float error = targetAltitude - currentAlt;
  // ... PID calculation
  return correction;
}

// 4. Apply to throttle
throttle = baseThrottle + altitudePID_output;
```

**Files to Modify**:
- `firmware/flightController/flightController.ino`

**Testing**:
- Verify sensor readings accurate (measure manually)
- Test with drone secured (no flight)
- Gradually increase altitude gains
- Test in safe environment

---

### FEATURE-002: Enable and Tune Yaw Control
**Priority**: 🟡 HIGH
**Estimated Time**: 2-3 hours

**Current State**: All yaw gains set to 0

**Implementation**:
1. Set initial yaw gains:
   ```cpp
   pidPGainYaw = 2.0;
   pidIGainYaw = 0.01;
   pidDGainYaw = 0.0;
   ```

2. Fix yaw angle reading (currently not updated from IMU)
   ```cpp
   // In calculate_pid():
   angleYaw = MPU.getAngleZ();
   ```

3. Add yaw rate limiting for safety

4. Tune using methods in PID_TUNING.md

**Considerations**:
- Yaw typically needs higher P-gain than roll/pitch
- Yaw I-gain prevents drift during hover
- D-gain often not needed for yaw

---

### FEATURE-003: Improve IMU Filtering
**Priority**: 🟡 HIGH
**Estimated Time**: 3-4 hours

**Current State**: Basic averaging (99.96% gyro, 0.04% accel)

**Improvements**:

1. **Implement Complementary Filter**:
   ```cpp
   float alpha = 0.98;  // Gyro trust factor
   float dt = 0.01;     // Loop time (seconds)

   angleRoll = alpha * (angleRoll + gyroX * dt) +
               (1 - alpha) * accelAngleX;
   ```

2. **Add Kalman Filter** (advanced):
   - Better noise rejection
   - Optimal sensor fusion
   - Requires matrix math library

3. **Low-pass Filter for D-term**:
   ```cpp
   float dFilter = 0.3;
   filteredDErr = dFilter * dErr + (1 - dFilter) * lastFilteredDErr;
   ```

**Benefits**:
- Reduced noise in angle estimates
- Smoother motor control
- Better D-term performance

---

### FEATURE-004: Add Deadband to Controls
**Priority**: 🟢 MEDIUM
**Estimated Time**: 1 hour

**Purpose**: Ignore small control inputs for stable hover

**Implementation**:
```cpp
float applyDeadband(float input, float deadband) {
  if (abs(input) < deadband) {
    return 0;
  }
  return input;
}

// In main loop:
float rollInput = applyDeadband(roll_Throttle - 1500, 10);
float pitchInput = applyDeadband(pitch_Throttle - 1500, 10);
```

**Benefits**:
- Prevents drift from stick centering errors
- Reduces jitter in hover
- Easier to maintain stable position

---

## Phase 3: Code Quality & Safety 🟢

### REFACTOR-001: Modularize Flight Controller Code
**Priority**: 🟢 MEDIUM
**Estimated Time**: 4-6 hours

**Goal**: Split monolithic .ino file into logical modules

**Proposed Structure**:
```
firmware/flightController/
├── flightController.ino      # Main loop and setup
├── config.h                  # All configuration constants
├── pid_controller.h          # PID calculation functions
├── motor_control.h           # Motor mixing and output
├── sensor_reading.h          # IMU and ultrasonic functions
└── serial_commands.h         # Input processing
```

**Benefits**:
- Easier to understand and modify
- Better code reusability
- Simplified testing
- Clearer separation of concerns

---

### REFACTOR-002: Add Comprehensive Logging
**Priority**: 🟢 MEDIUM
**Estimated Time**: 2-3 hours

**Implementation**:
```cpp
// Add structured logging
void logTelemetry() {
  Serial.print("T:");  Serial.print(millis());
  Serial.print(",ALT:"); Serial.print(altitude);
  Serial.print(",R:"); Serial.print(angleRoll);
  Serial.print(",P:"); Serial.print(anglePitch);
  Serial.print(",Y:"); Serial.print(angleYaw);
  Serial.print(",THR:"); Serial.print(throttle);
  Serial.println();
}

// Call every 100ms
if (millis() - lastLog > 100) {
  logTelemetry();
  lastLog = millis();
}
```

**Analysis Tools**:
- Parse logs with Python script
- Plot data in matplotlib
- Identify oscillations and trends
- Tune PIDs offline

---

### SAFETY-001: Add Pre-flight Checks
**Priority**: 🟡 HIGH
**Estimated Time**: 2 hours

**Checks to Implement**:
```cpp
bool preflight_check() {
  bool passed = true;

  // 1. IMU responding
  if (!MPU.update()) {
    Serial.println("ERROR: IMU not responding");
    passed = false;
  }

  // 2. Battery voltage (if monitoring added)
  if (batteryVoltage < 10.5) {
    Serial.println("ERROR: Battery too low");
    passed = false;
  }

  // 3. All motors responding
  // Test each motor briefly

  // 4. Sensor calibration
  if (abs(angleRoll) > 5 || abs(anglePitch) > 5) {
    Serial.println("WARNING: Drone not level");
  }

  return passed;
}
```

**Safety Features**:
- Won't arm if checks fail
- Visual/audio indicators
- Automatic disarm on critical errors

---

### SAFETY-002: Add Failsafe Mechanisms
**Priority**: 🟡 HIGH
**Estimated Time**: 3 hours

**Failsafes to Implement**:

1. **Communication Loss**:
   ```cpp
   unsigned long lastCommand = 0;

   if (millis() - lastCommand > 1000) {
     // No command for 1 second - land safely
     throttle = max(throttle - 5, 1000);
   }
   ```

2. **Extreme Angle Protection**:
   ```cpp
   if (abs(angleRoll) > 45 || abs(anglePitch) > 45) {
     // Cut motors if upside down
     emergency_stop();
   }
   ```

3. **Battery Monitoring**:
   ```cpp
   if (batteryVoltage < 10.2) {  // 3.4V per cell
     // Force landing
     land_mode = true;
   }
   ```

4. **Altitude Limits**:
   ```cpp
   if (altitude > 300) {  // 3m max with HC-SR04
     throttle = min(throttle, 1500);
   }
   ```

---

## Phase 4: Enhanced Features 🔵

### FEATURE-005: Add Flight Modes
**Priority**: 🔵 LOW
**Estimated Time**: 4-6 hours

**Modes**:
1. **MANUAL**: Direct motor control (testing only)
2. **STABILIZE**: Current mode (attitude hold)
3. **ALTITUDE_HOLD**: Maintain height automatically
4. **POSITION_HOLD**: GPS-based position lock (requires GPS)
5. **RETURN_TO_LAUNCH**: Autonomous return (requires GPS)

**Implementation**:
```cpp
enum FlightMode {
  MANUAL,
  STABILIZE,
  ALTITUDE_HOLD,
  POSITION_HOLD,
  RETURN_TO_LAUNCH
};

FlightMode currentMode = STABILIZE;

// Switch via serial command
if (input == 'm') currentMode = MANUAL;
if (input == 's') currentMode = STABILIZE;
if (input == 'h') currentMode = ALTITUDE_HOLD;
```

---

### FEATURE-006: Wireless Control
**Priority**: 🔵 LOW
**Estimated Time**: 6-8 hours

**Options**:

1. **Bluetooth Module** (HC-05/HC-06)
   - Short range (~10m)
   - Easy implementation
   - Low cost

2. **WiFi (ESP8266/ESP32)**
   - Medium range (~50m)
   - Can add web interface
   - More complex

3. **RF Transmitter** (NRF24L01)
   - Long range (~100m)
   - Low latency
   - Requires custom receiver

**Benefits**:
- Untethered flight
- Longer range
- Better mobility

---

### FEATURE-007: Telemetry Downlink
**Priority**: 🔵 LOW
**Estimated Time**: 4-6 hours

**Data to Transmit**:
- Battery voltage
- Altitude
- Attitude (roll, pitch, yaw)
- GPS position (if added)
- PID outputs
- Motor speeds

**Display Options**:
1. Update Python controller with real-time display
2. Create GUI with PyQt or Tkinter
3. Web-based interface (if using WiFi)

---

### FEATURE-008: GPS Integration
**Priority**: 🔵 LOW
**Estimated Time**: 8-10 hours

**Hardware Needed**:
- GPS module (NEO-6M or better)
- Additional Arduino pins

**Features Enabled**:
- Position hold
- Return to launch
- Waypoint navigation
- Geofencing

**Challenges**:
- Arduino Uno limited pins
- May need to upgrade to Mega or Teensy
- GPS lock can be slow
- Altitude from GPS is inaccurate

---

## Phase 5: Hardware Upgrades 🔵

### HARDWARE-001: Upgrade to Better IMU
**Priority**: 🔵 LOW
**Cost**: $15-30

**Options**:
- **BNO055**: Built-in sensor fusion, quaternion output
- **ICM-20948**: 9-DOF (gyro + accel + mag)
- **MPU9250**: 9-DOF, better specs than MPU6050

**Benefits**:
- Better accuracy
- Built-in filtering
- Magnetometer for heading
- Reduced drift

---

### HARDWARE-002: Add Optical Flow Sensor
**Priority**: 🔵 LOW
**Cost**: $20-40

**Options**:
- **PMW3901**: Low-altitude optical flow
- **PX4FLOW**: Full camera-based flow

**Benefits**:
- Indoor position hold (no GPS needed)
- Drift compensation
- Better hovering stability

**Limitations**:
- Requires downward-facing camera
- Limited to low altitude (<5m)
- Needs textured surface

---

### HARDWARE-003: Upgrade to More Powerful MCU
**Priority**: 🔵 LOW
**Cost**: $10-50

**Reasoning**: Arduino Uno is limited
- Only 2KB RAM
- 16 MHz clock
- Limited pins
- No floating-point unit

**Options**:
- **Arduino Mega** ($40) - More pins, same architecture
- **Teensy 4.0** ($20) - 600 MHz ARM, floating-point
- **ESP32** ($10) - WiFi built-in, dual-core
- **STM32** ($15) - Common in flight controllers

**Benefits**:
- Faster loop rates (>1kHz possible)
- More complex algorithms
- More sensors
- Better performance

---

## Testing Plan

### Test 1: Firmware Bug Fixes (No Flight)
**Goal**: Verify all critical bugs fixed

**Procedure**:
1. Fix BUG-001 through BUG-006
2. Compile and upload to Arduino
3. Test PID calculations with Serial output
4. Verify Increment/Decrement work
5. Test Python controller connects

**Success Criteria**:
- [ ] Code compiles without errors
- [ ] PID I-term accumulates correctly
- [ ] Output clamping works both directions
- [ ] Keyboard inputs change throttle
- [ ] No crashes or unexpected behavior

---

### Test 2: Tethered Stability Test
**Goal**: Verify basic stability without full flight

**Procedure**:
1. Remove propellers
2. Secure drone to test stand (allows rotation but not translation)
3. Power on and arm
4. Apply manual roll/pitch commands
5. Observe motor response

**Success Criteria**:
- [ ] Motors respond to attitude changes
- [ ] Corrections in right direction
- [ ] No oscillations at rest
- [ ] Emergency stop works

---

### Test 3: Low Hover Test
**Goal**: First flight - stability at 0.5m

**Procedure**:
1. Install propellers (correct orientation!)
2. Open area, no obstacles
3. Gradually increase throttle
4. Hover at 0.5m for 30 seconds
5. Land smoothly

**Success Criteria**:
- [ ] Stable hover (±0.2m position)
- [ ] No oscillations
- [ ] Responds to manual inputs
- [ ] Controlled landing

---

### Test 4: Altitude Hold Test
**Goal**: Verify altitude PID works

**Procedure**:
1. Enable altitude hold mode
2. Set target altitude (1.0m)
3. Hover hands-off for 60 seconds
4. Manually disturb (gently push down)
5. Verify returns to target altitude

**Success Criteria**:
- [ ] Maintains altitude ±0.1m
- [ ] Corrects disturbances
- [ ] No oscillations in height
- [ ] Smooth adjustments

---

## Success Metrics

### Minimum Viable Product (MVP)
- [ ] All critical bugs fixed
- [ ] Stable hover for 60 seconds
- [ ] Responds to manual controls
- [ ] Emergency stop reliable

### Stable Hovering Goal
- [ ] Altitude hold maintains ±5cm
- [ ] Position drift <0.5m over 60s
- [ ] No oscillations
- [ ] Flight time >5 minutes
- [ ] Yaw control functional

### Stretch Goals
- [ ] Position hold (GPS or optical flow)
- [ ] Autonomous takeoff/landing
- [ ] Waypoint navigation
- [ ] 10+ minute flight time
- [ ] Return to launch

---

## Resources Needed

### Immediate (Phase 1-2)
- [ ] Time: 10-15 hours
- [ ] Cost: $0 (bug fixes only)
- [ ] Tools: Arduino IDE, Python, multimeter

### Near-term (Phase 3-4)
- [ ] Time: 20-30 hours
- [ ] Cost: $20-50 (GPS, telemetry radio)
- [ ] Tools: Oscilloscope (optional), logic analyzer (optional)

### Long-term (Phase 5)
- [ ] Time: 40+ hours
- [ ] Cost: $100-200 (new MCU, sensors, frame upgrades)
- [ ] Tools: 3D printer (optional), better soldering equipment

---

## Contributing

Want to help improve AutoDrone? Here are the best places to start:

### Good First Issues
1. Fix BUG-001 (PID integral term)
2. Fix BUG-004 (Python self parameter)
3. Add deadband to controls (FEATURE-004)

### Medium Difficulty
1. Implement altitude hold (FEATURE-001)
2. Enable yaw control (FEATURE-002)
3. Add telemetry logging (REFACTOR-002)

### Advanced
1. Implement Kalman filter (FEATURE-003)
2. Modularize codebase (REFACTOR-001)
3. Add GPS support (FEATURE-008)

---

**Last Updated**: 2025-11-15
**Status**: Planning Phase - Ready for Implementation
