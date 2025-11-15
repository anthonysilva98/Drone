# PID Tuning Guide

Complete guide for tuning the AutoDrone PID controllers to achieve stable hovering flight.

## Table of Contents
1. [Understanding PID Control](#understanding-pid-control)
2. [Current PID Implementation](#current-pid-implementation)
3. [Before You Start](#before-you-start)
4. [Tuning Procedure](#tuning-procedure)
5. [Common Problems](#common-problems)
6. [Advanced Tuning](#advanced-tuning)

## Understanding PID Control

### What is PID?

PID (Proportional-Integral-Derivative) is a control loop mechanism that continuously calculates an error value and applies corrections to minimize that error.

```
Error = Setpoint - Current_Value

Output = (P × Error) + (I × ∫Error dt) + (D × dError/dt)
```

### The Three Terms

#### Proportional (P)
- **What it does**: Responds proportionally to current error
- **Effect**: Larger error = stronger correction
- **Problem**: Can cause steady-state error, never quite reaching target
- **Analogy**: Pushing harder on gas pedal the further you are from target speed

#### Integral (I)
- **What it does**: Accumulates error over time
- **Effect**: Eliminates steady-state error
- **Problem**: Can cause overshoot and wind-up
- **Analogy**: Remembering how long you've been below target speed

#### Derivative (D)
- **What it does**: Responds to rate of change of error
- **Effect**: Dampens oscillations, predicts future error
- **Problem**: Sensitive to noise, can cause instability
- **Analogy**: Anticipating when to ease off gas before reaching target

### PID in Quadcopters

Quadcopters typically use cascaded PID loops:

```
┌─────────────┐
│   Pilot     │
│   Input     │
└──────┬──────┘
       │
       ▼
┌─────────────┐     ┌─────────────┐
│   Attitude  │────▶│   Rate      │
│   PID       │     │   PID       │
│ (degrees)   │     │ (deg/sec)   │
└─────────────┘     └──────┬──────┘
                           │
                           ▼
                    ┌─────────────┐
                    │   Motors    │
                    └─────────────┘
```

**Note**: Our current implementation uses a single-loop attitude PID (simplified for hackathon project).

## Current PID Implementation

### Current Gains (Default)

#### Roll & Pitch
```cpp
P-gain: 1.0
I-gain: 0.04
D-gain: 0.0 (adjustable via keys 1/2/3)
Max Output: 2000
```

#### Yaw
```cpp
P-gain: 0.0 (DISABLED)
I-gain: 0.0 (DISABLED)
D-gain: 0.0 (DISABLED)
Max Output: 2000
```

### Known Issues in Current Code

**CRITICAL**: Fix these bugs before tuning!

1. **Integral Term Bug** (Lines 60, 70, 80)
   ```cpp
   // WRONG:
   pidIMemRoll += pidIGainRoll + pidErrTemp;

   // CORRECT:
   pidIMemRoll += pidIGainRoll * pidErrTemp;
   ```

2. **Output Clamping Bug** (Lines 66, 76, 86)
   ```cpp
   // WRONG:
   else if (pidOutputRoll < pidMaxRoll) pidOutputRoll = pidMaxRoll * -1;

   // CORRECT:
   else if (pidOutputRoll < -pidMaxRoll) pidOutputRoll = -pidMaxRoll;
   ```

3. **Variable Name Typo** (Line 41)
   ```cpp
   // WRONG:
   float pidIGainPitchaw = 0.;

   // CORRECT:
   float pidIGainYaw = 0.;
   ```

## Before You Start

### Safety Requirements

- [ ] Remove propellers for initial testing
- [ ] Test in open area (minimum 10m radius)
- [ ] Have emergency stop ready ('P' key)
- [ ] Fully charged battery
- [ ] Secure mounting of IMU sensor
- [ ] IMU calibrated (place flat during startup)
- [ ] Test USB cable won't interfere with flight

### Understanding Your System

1. **Mass**: Heavier drones need higher P-gain
2. **Motor Response**: Faster motors can handle higher D-gain
3. **Frame Rigidity**: Flexible frames may oscillate more
4. **Propeller Size**: Larger props = slower response
5. **IMU Position**: Should be at center of gravity

### Test Equipment

- Laptop with ground control software
- USB cable (long enough for test flight)
- Fully charged battery
- Spare propellers
- Notebook for recording gain values

## Tuning Procedure

### Method 1: Ziegler-Nichols (Classic)

This method finds the "ultimate gain" where system oscillates continuously.

#### Step 1: Find Ultimate Gain (Ku)
1. Set I-gain = 0, D-gain = 0
2. Start with very low P-gain (e.g., 0.1)
3. Gradually increase P-gain until drone oscillates steadily
4. Record this value as **Ku** (ultimate gain)
5. Measure oscillation period **Tu** (time for one complete oscillation)

#### Step 2: Calculate Gains

For **Classic PID**:
```
P-gain = 0.6 × Ku
I-gain = 1.2 × Ku / Tu
D-gain = 0.075 × Ku × Tu
```

For **Less Overshoot**:
```
P-gain = 0.33 × Ku
I-gain = 0.66 × Ku / Tu
D-gain = 0.11 × Ku × Tu
```

For **No Overshoot**:
```
P-gain = 0.2 × Ku
I-gain = 0.4 × Ku / Tu
D-gain = 0.066 × Ku × Tu
```

### Method 2: Manual Tuning (Recommended for Beginners)

This is safer and more intuitive for first-time tuning.

#### Phase 1: P-Gain Only

1. **Start Conservative**
   ```
   P-gain = 0.5
   I-gain = 0.0
   D-gain = 0.0
   ```

2. **Arm drone and hover at low altitude** (0.5m)

3. **Increase P-gain gradually** (steps of 0.1-0.2)
   - Too low: Drone is sluggish, slow to correct
   - Just right: Quick response, slight overshoot
   - Too high: Rapid oscillations

4. **Target**: Find P-gain where drone responds quickly but oscillates slightly

5. **Back off**: Reduce P-gain by 20% from oscillation point

**Example**: If oscillations start at P = 2.0, use P = 1.6

#### Phase 2: Add I-Gain

1. **Start with very small I-gain**
   ```
   P-gain = [value from Phase 1]
   I-gain = 0.01
   D-gain = 0.0
   ```

2. **Test hover for 30 seconds**
   - Watch for drift correction
   - Monitor for wind-up (increasing oscillations over time)

3. **Increase I-gain gradually** (steps of 0.01-0.02)
   - Too low: Drone drifts with wind or balance shifts
   - Just right: Holds position, no drift
   - Too high: Slow oscillations that grow over time

4. **Target**: Smallest I-gain that eliminates steady-state error

**Typical range**: 0.02 - 0.08

#### Phase 3: Add D-Gain

1. **Start with zero D-gain**
   ```
   P-gain = [value from Phase 1]
   I-gain = [value from Phase 2]
   D-gain = 0.0
   ```

2. **Make a quick tilt correction** (W or S key tap)
   - Observe overshoot behavior
   - Note if oscillations dampen quickly

3. **Use real-time D-gain tuning** (keys 1, 2, 3)
   - Press '1' to increment D-gain (+1.0)
   - Press '2' to decrement D-gain (-1.0)
   - Press '3' to reduce by 25% (× 0.75)

4. **Increase D-gain until oscillations stop**
   - Too low: Overshoots, bounces back and forth
   - Just right: Smooth return to level, no overshoot
   - Too high: Twitchy, high-frequency vibrations

**Typical range**: 10-50 (much larger than P/I for angular rates)

### Method 3: Recommended Starting Values

Based on typical Arduino quadcopter builds:

#### Conservative (Start Here)
```cpp
// Roll & Pitch
pidPGainRoll = 0.8;
pidIGainRoll = 0.03;
pidDGainRoll = 15.0;

// Yaw (enable after roll/pitch stable)
pidPGainYaw = 2.0;
pidIGainYaw = 0.01;
pidDGainYaw = 0.0;
```

#### Moderate (After Testing)
```cpp
// Roll & Pitch
pidPGainRoll = 1.2;
pidIGainRoll = 0.05;
pidDGainRoll = 25.0;

// Yaw
pidPGainYaw = 3.0;
pidIGainYaw = 0.02;
pidDGainYaw = 0.5;
```

#### Aggressive (Experienced Pilots)
```cpp
// Roll & Pitch
pidPGainRoll = 1.8;
pidIGainRoll = 0.08;
pidDGainRoll = 35.0;

// Yaw
pidPGainYaw = 4.0;
pidIGainYaw = 0.03;
pidDGainYaw = 1.0;
```

## Common Problems

### Problem: Rapid Oscillations (>5 Hz)

**Symptoms**: High-frequency shaking, buzzing sound

**Causes**:
- P-gain too high
- D-gain too high
- IMU mounted on vibrating surface

**Solutions**:
1. Reduce P-gain by 30%
2. Reduce D-gain by 50%
3. Add vibration dampening to IMU
4. Check for loose screws or components

### Problem: Slow Oscillations (0.5-2 Hz)

**Symptoms**: Gentle rocking back and forth

**Causes**:
- P-gain too low
- I-gain too high
- Delayed response (low loop rate)

**Solutions**:
1. Increase P-gain by 20%
2. Reduce I-gain by 50%
3. Optimize code for faster loop times
4. Reduce Serial.print() statements

### Problem: Drone Flips on Takeoff

**Symptoms**: Immediate flip or uncontrolled roll

**Causes**:
- Motor direction wrong
- Propeller orientation wrong
- PID output inverted
- IMU orientation wrong

**Solutions**:
1. Verify motor layout matches X-configuration
2. Check propeller directions (CW vs CCW)
3. Verify PID signs in motor mixing
4. Ensure IMU is right-side-up and level

### Problem: Steady Drift

**Symptoms**: Slowly drifts in one direction

**Causes**:
- I-gain too low
- IMU not calibrated
- Frame not balanced
- Wind or air currents

**Solutions**:
1. Increase I-gain slightly
2. Recalibrate IMU (place flat on startup)
3. Balance the frame (CG at geometric center)
4. Fly in calmer conditions

### Problem: Increasing Oscillations

**Symptoms**: Starts stable, gets worse over time

**Causes**:
- I-gain wind-up
- No integral clamping
- Accumulating error

**Solutions**:
1. Reduce I-gain
2. Check I-term limits (pidMaxRoll)
3. Reset I-term on large errors
4. Add anti-windup logic

### Problem: Twitchy, Nervous Response

**Symptoms**: Constant small corrections, never settles

**Causes**:
- D-gain too high
- Noisy IMU readings
- Poor IMU filtering

**Solutions**:
1. Reduce D-gain by 30%
2. Implement complementary filter for IMU
3. Add low-pass filter to D-term
4. Reduce sensor noise (better mounting)

## Advanced Tuning

### Integral Wind-up Prevention

Add reset logic:
```cpp
// Reset I-term if error is very large
if (abs(pidErrTemp) > 45) {
  pidIMemRoll = 0;
}

// Reset I-term when not flying
if (throttle < 1100) {
  pidIMemRoll = 0;
  pidIMemPitch = 0;
  pidIMemYaw = 0;
}
```

### Derivative Filtering

Add low-pass filter to reduce D-term noise:
```cpp
float dTermAlpha = 0.3;  // Filter coefficient (0-1)
float filteredDErr;

filteredDErr = dTermAlpha * (pidErrTemp - pidLastDErrRoll) +
               (1 - dTermAlpha) * filteredDErr;

pidOutputRoll = pidPGainRoll * pidErrTemp +
                pidIMemRoll +
                pidDGainRoll * filteredDErr;
```

### Gain Scheduling

Adjust gains based on throttle:
```cpp
float throttlePercent = throttle / 2000.0;
float adjustedPGain = pidPGainRoll * (0.8 + 0.4 * throttlePercent);
```

### Setpoint Weighting

Reduce P-term influence on setpoint changes:
```cpp
float setpointWeight = 0.6;  // 0.0 to 1.0
float pTerm = pidPGainRoll * (setpointWeight * pidSetpointRoll - angleRoll);
```

## Tuning Log Template

Keep track of your tuning sessions:

```
Date: ________
Battery: _____ mAh, _____ V
Total Weight: _____ g
Propellers: _____ inch

Test #  | P-gain | I-gain | D-gain | Result
--------|--------|--------|--------|---------------------------
1       |  0.5   |  0.0   |  0.0   | Too slow, no oscillation
2       |  1.0   |  0.0   |  0.0   | Good response, slight overshoot
3       |  1.5   |  0.0   |  0.0   | Fast oscillations - too high
4       |  1.0   |  0.03  |  0.0   | Holds position better
5       |  1.0   |  0.03  | 15.0   | Reduced overshoot
--------|--------|--------|--------|---------------------------
FINAL:  |  1.0   |  0.03  | 15.0   | STABLE
```

## References and Further Reading

1. **PID Theory**
   - "PID Control Theory" - Wikipedia
   - "Understanding PID Control" - National Instruments

2. **Quadcopter-Specific**
   - "How to Tune PID Loops" - Oscar Liang
   - "Quadcopter PID Explained" - Andrew Gibiansky

3. **Arduino Flight Controllers**
   - YMFC-AL (Joop Brokking)
   - MultiWii Project
   - Arduino PID Library documentation

## Safety Reminder

**ALWAYS**:
- Remove propellers for initial testing
- Test in open area
- Have emergency stop ready
- Monitor battery voltage
- Start with conservative gains
- Increase gains gradually

**NEVER**:
- Tune with people or obstacles nearby
- Make large gain changes during flight
- Continue flying with unstable behavior
- Ignore oscillations or vibrations

---

**Good luck with tuning! Take your time, be methodical, and prioritize safety.**
