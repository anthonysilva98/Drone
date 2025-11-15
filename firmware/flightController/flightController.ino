#include <Keyboard.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 MPU(Wire);
char input;
// Ints
int incoming = 0;
int valx, thrust, fl_thrust, fr_thrust, bl_thrust, br_thrust;
int fl = 3, fr = 6, bl = 10, br = 11;
int desired_height = 12;
int trigPin = 4, echoPin = 5;
int i = 1;
int throttle, pitch_Throttle, roll_Throttle, yaw_Throttle;

// Floats
float anglePitch, angleRoll, angleYaw, last_angleRoll, last_anglePitch;
float GyroX, GyroY, AccX, AccY;
float pidLastDErrPitch, pidLastDErrYaw, pidLastDErrRoll, pidErrTemp;
float pidOutputPitch, pidOutputYaw, pidOutputRoll;
// float pidIGainPitch, pidIGainYaw, pidIGainRoll;
float pidIGainYaw;
float pidIMemPitch, pidIMemYaw, pidIMemRoll;
// float pidSetpointPitch, pidSetpointYaw, pidSetpointRoll;
float LevelAdjPitch, LevelAdjRoll;

// PID values for roll.
float pidPGainRoll = 1.;//take 50%
float pidIGainRoll = 0.04;
float pidDGainRoll = 0;//take 25%
float pidMaxRoll = 2000;
float pidSetpointRoll = 0;
// PID values for pitch.
float pidPGainPitch = pidPGainRoll;
float pidIGainPitch = pidIGainRoll;
float pidDGainPitch = pidDGainRoll;
float pidMaxPitch = pidMaxRoll;
float pidSetpointPitch = 0;
// PID values for yaw.
float pidPGainYaw = 0;//4
float pidIGainPitchaw = 0.;//.02
float pidDGainYaw = 0.0;
float pidMaxYaw = pidMaxRoll;
float pidSetpointYaw = 0;


void calculate_pid() {
  MPU.update();
  // Read values from MPU6050 and deal with noise.
angleRoll = MPU.getAngleX();
angleRoll = (angleRoll * .9996) + (MPU.getAccX() * .0004);
anglePitch = MPU.getAngleY();
anglePitch = (anglePitch * .9996) + (MPU.getAccY() * .0004);

LevelAdjRoll = angleRoll * 15;
LevelAdjPitch = anglePitch * 15;

// Roll Calculations
pidErrTemp = angleRoll - pidSetpointRoll;
pidIMemRoll += pidIGainRoll * pidErrTemp;
if (pidIMemRoll > pidMaxRoll) pidIMemRoll = pidMaxRoll;
else if (pidIMemRoll < pidMaxRoll * -1) pidIMemRoll = pidMaxRoll * -1;

pidOutputRoll = pidPGainRoll * pidErrTemp + pidIMemRoll + pidDGainRoll * (pidErrTemp - pidLastDErrRoll);
if (pidOutputRoll > pidMaxRoll) pidOutputRoll = pidMaxRoll;
else if (pidOutputRoll < -pidMaxRoll) pidOutputRoll = -pidMaxRoll;

// Pitch Calculations
pidErrTemp = anglePitch - pidSetpointPitch;
pidIMemPitch += pidIGainPitch * pidErrTemp;
if (pidIMemPitch > pidMaxPitch) pidIMemPitch = pidMaxPitch;
else if (pidIMemPitch < pidMaxPitch) pidIMemPitch = pidMaxPitch * -1;

pidOutputPitch = pidPGainPitch * pidErrTemp + pidIMemPitch + pidDGainPitch * (pidErrTemp - pidLastDErrPitch);
if (pidOutputPitch > pidMaxPitch) pidOutputPitch = pidMaxPitch;
else if (pidOutputPitch < -pidMaxPitch) pidOutputPitch = -pidMaxPitch;

// Yaw Calculations
pidErrTemp = angleYaw - pidSetpointYaw;
pidIMemYaw += pidIGainYaw * pidErrTemp;
if (pidIMemYaw > pidMaxYaw) pidIMemYaw = pidMaxYaw;
else if (pidIMemYaw < pidMaxYaw) pidIMemYaw = pidMaxYaw * -1;

pidOutputYaw = pidPGainYaw * pidErrTemp + pidIMemYaw + pidDGainYaw * (pidErrTemp - pidLastDErrYaw);
if (pidOutputYaw > pidMaxYaw) pidOutputYaw = pidMaxYaw;
else if (pidOutputYaw < -pidMaxYaw) pidOutputYaw = -pidMaxYaw;
pidLastDErrYaw = pidErrTemp;
}


void getInput() {
  while (Serial.available() == 0) {}
  incoming = Serial.read();
  input = (char)incoming;
   switch (input) {
    case 'p':
      setThrust(0, 0, 0, 0);
      break;
   }
  switch (input) {
    case 'w':
      Increment(pitch_Throttle);
      Serial.println(input);
      break;
    
    case 's': 
      Decrement(pitch_Throttle);
         Serial.println(input);
      break;
    
    default: 
    pitch_Throttle = 1500;
    break;
  }

  switch (input) {
    case 'd':
      Increment(roll_Throttle);
         Serial.println(input);
      break;
    
    case 'a':
       Decrement(roll_Throttle);
          Serial.println(input);
      break;
    
    default: 
    roll_Throttle = 1500;
    break;
  }

  switch (input) {
    case 'e':
      Increment(yaw_Throttle);
         Serial.println(input);
      break;
    
    case 'q':
      Decrement(yaw_Throttle);
         Serial.println(input);
      break;
      default: break;
  }
        switch (input) {
    case 'i':
    Increment(thrust);
       Serial.println(input);
      break;
    
    case 'k':
    Decrement(throttle);
       Serial.println(input);
      break;
    
    default:break;
    
  }
   switch (input) {
    case '1':
    pidDGainRoll += 1;
     pidDGainPitch = pidDGainRoll;
         Serial.println(pidDGainRoll);

      break;
   }
   switch (input) {
    case '2':
    pidDGainRoll -= 1;
    pidDGainPitch = pidDGainRoll;
        Serial.println(pidDGainRoll);
        break;
   }
   switch (input) {
    case '3':
    pidDGainRoll = pidDGainRoll * .75;
    pidDGainPitch = pidDGainRoll;
    Serial.println(pidDGainRoll);
      break;
   }
}


void setup()
{
  Serial.begin(57600);
  Wire.begin();
  MPU.begin();
  MPU.calcGyroOffsets(true);
  init_esc();

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}


void loop() {  
getInput();
Serial.println("Passed getInput call.");

calculate_pid();  // Calculate PID for next roll.

if (pitch_Throttle > 1508) pidSetpointPitch = pitch_Throttle - 1508;
else if (pitch_Throttle < 1492) pidSetpointPitch = pitch_Throttle - 1492;

if (roll_Throttle > 1508) pidSetpointRoll = roll_Throttle - 1508;
else if (roll_Throttle < 1492) pidSetpointRoll = roll_Throttle - 1492;

if (yaw_Throttle > 1508) pidSetpointYaw = yaw_Throttle - 1508;
else if (yaw_Throttle < 1492) pidSetpointYaw = yaw_Throttle - 1492;

Serial.println(pidOutputPitch);
Serial.println(pidOutputRoll);
Serial.println(pidOutputYaw);
Serial.println(throttle);

fl_thrust = throttle - pidOutputPitch - pidOutputRoll + pidOutputYaw;
fr_thrust = throttle - pidOutputPitch + pidOutputRoll - pidOutputYaw;
bl_thrust = throttle + pidOutputPitch - pidOutputRoll - pidOutputYaw;
br_thrust = throttle + pidOutputPitch + pidOutputRoll + pidOutputYaw;

setThrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);

}


void init_esc(){
 delay(3000);
 thrust = 254;
 Serial.println(thrust);
 analogWrite(fl, thrust);
 analogWrite(fr, thrust);
 analogWrite(bl, thrust);
 analogWrite(br, thrust);
 delay(1000);
 
 thrust = 1;
 Serial.println(thrust);
 analogWrite(fl, thrust);
 analogWrite(fr, thrust);
 analogWrite(bl, thrust);
 analogWrite(br, thrust);
 delay(1000);
}


void setThrust(int fl_t, int fr_t, int bl_t, int br_t) {
// fl_t = map(fl_t, 0, 2000, 1, 254);
// fr_t = map(fr_t, 0, 2000, 1, 254);
// bl_t = map(bl_t, 0, 2000, 1, 254);
// br_t = map(br_t, 0, 2000, 1, 254);
 Serial.println(fl_t);
 Serial.println(fr_t);
 Serial.println(bl_t);
 Serial.println(fl_t);
 analogWrite(fl, fl_t);
 analogWrite(fr, fr_t);
 analogWrite(bl, bl_t);
 analogWrite(br, br_t);
}
void Increment(int valToIncrement)
{
  if(valToIncrement > 2000)
  valToIncrement = 2000;
  else
   valToIncrement++;
  
}
void Decrement(int valToDecrement)
{
  if(valToDecrement < 0)
  valToDecrement = 0;
  else
    valToDecrement--;
}