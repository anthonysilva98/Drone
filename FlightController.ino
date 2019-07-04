#include <Keyboard.h>
#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 MPU(Wire);

int valx, fl_thrust, fr_thrust, bl_thrust, br_thrust;
int fl = 3, fr = 6, bl = 10, br = 11;
int desired_height = 12;
int trigPin = 4, echoPin = 5;
int i = 1;

float angleRoll, anglePitch, last_angleRoll, last_anglePitch;
float GyroX, GyroY, AccX, AccY;
float pidSetpointRoll, pidSetpointPitch, pidSetpointYaw;

// PID values for roll.
float pidPGainRoll = 1.;
float pidIGainRoll = 0.04;
float pidDGainRoll = 18;
float pidMaxRoll = 400;
// PID values for pitch.
float pidPGainPitch = pidPGainRoll;
float pidIGainPitch = pidIGainRoll;
float pidDGainPitch = pidDGainRoll;
float pidMaxPitch = pidMaxRoll;
// PID values for yaw.
float pidPGainYaw = 4.0;
float pidIGainPitchaw = 0.02;
float pidDGainYaw = 0.0;
float pidMaxYaw = pidMaxRoll;

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  MPU.begin();
  MPU.calcGyroOffsets(true);
  init_esc();

  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
}


void loop() {  
Serial.println("Enter in a motor ");
while (Serial.available() == 0)
{}
  valx = Serial.parseInt();
if(valx == 1)
fl_thrust += 1;
analogWrite(fl,fl_thrust);
if(valx == 2){
fr_thrust += 1 ;
analogWrite(fr,fr_thrust);
}
if(valx == 3)
{
bl_thrust += 1;
analogWrite(bl,bl_thrust);
}
if(valx == 4)
{
br_thrust += 1;
analogWrite(br,br_thrust);
}
if(valx == 5){
fl_thrust += 5;
fr_thrust += 5;
bl_thrust += 5;
br_thrust += 5;
 analogWrite(fl, fl_thrust);
 analogWrite(fr, fr_thrust);
 analogWrite(bl, bl_thrust);
 analogWrite(br, br_thrust);

}

 Serial.println(thrust);
 Serial.println(fl_thrust);
 Serial.println(fr_thrust);
 Serial.println(bl_thrust);
 Serial.println(br_thrust);
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
//bool is_thrust_max(){
//  return (fl_thrust < 255 && fr_thrust < 255 && bl_thrust < 255 && br_thrust < 255);
//}
//
//bool is_thrust_min(){
//  return (fl_thrust > 0 && fr_thrust > 0 && bl_thrust > 0 && br_thrust > 0);
//}


void calculate_pid() {
  // Read values from MPU6050 and deal with noise.
angleRoll = MPU.getAngleX();
angleRoll = (angleRoll * .9996) + (MPU.getAccX() * .0004);
anglePitch = MPU.getAngleY();
anglePitch = (anglePitch * .9996) + (MPU.getAccY() * .0004);

LevelAdjRoll = angleRoll * 15;
LevelAdjPitch = anglePitch * 15;

// Roll Calculations
pidErrTemp = angleRoll - pidSetpointRoll;
pidIMemRoll += pidIGainRoll + pidErrTemp;
if (pidIMemRoll > pidMaxRoll) {
  pidIMemRoll = pidMaxRoll;
}
else if (pidIMemRoll < pidMaxRoll){
  pidIMemRoll = pidMaxRoll * -1;
}

pidOutputRoll = pidPGainRoll * pidErrTemp + pidIMemRoll + pidDGainRoll * (pidErrTemp - pidLastDErrRoll);
if (pidOutputRoll > pidMaxRoll) {
  pidOutputRoll = pidMaxRoll;
}
else if (pidOutputRoll < pidMaxRoll) {
  pidOutputRoll = pidMaxRoll * -1;
}

// Pitch Calculations
pidErrTemp = anglePitch - pidSetpointPitch;
pidIMemPitch += pidIGainPitch + pidErrTemp;
if (pidIMemPitch > pidMaxPitch) {
  pidIMemPitch = pidMaxPitch;
}
else if (pidIMemPitch < pidMaxPitch){
  pidIMemPitch = pidMaxPitch * -1;
}

pidOutputPitch = pidPGainPitch * pidErrTemp + pidIMemPitch + pidDGainPitch * (pidErrTemp - pidLastDErrPitch);
if (pidOutputPitch > pidMaxPitch) {
  pidOutputPitch = pidMaxPitch;
}
else if (pidOutputPitch < pidMaxPitch) {
  pidOutputPitch = pidMaxPitch * -1;
}

// Yaw Calculations
pidErrTemp = angleYaw - pidSetpointYaw;
pidIMemYaw += pidIGainYaw + pidErrTemp;
if (pidIMemYaw > pidMaxYaw) {
  pidIMemYaw = pidMaxYaw;
}
else if (pidIMemYaw < pidMaxYaw){
  pidIMemYaw = pidMaxYaw * -1;
}

pidOutputYaw = pidPGainYaw * pidErrTemp + pidIMemYaw + pidDGainYaw * (pidErrTemp - pidLastDErrYaw);
if (pidOutputYaw > pidMaxYaw) {
  pidOutputYaw = pidMaxYaw;
}
else if (pidOutputYaw < pidMaxYaw) {
  pidOutputYaw = pidMaxYaw * -1;
}
pidLastDErrYaw = pidErrTemp;
}
