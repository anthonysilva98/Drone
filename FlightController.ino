#include <MPU6050_tockn.h>
#include <Wire.h>
#include <Keyboard.h>
MPU6050 mpu6050(Wire);
int valx;
int fl = 3;
int fr = 6;
int bl = 10;
int br = 11;
int thrust;
int fl_thrust, fr_thrust, bl_thrust, br_thrust;
double angx, angy, less_angx, less_angy, more_angx, more_angy, start_angx, start_angy, var;
int desired_height = 12;
int trigPin = 4;
int echoPin = 5;
int array[10][7];
int i = 1;

void setup() {
 Serial.begin(9600);
fl_thrust = 80;
fr_thrust = 140;
bl_thrust = 80;
br_thrust = 80;
 pinMode(echoPin, INPUT);
 pinMode(trigPin,OUTPUT);
int height = 0;
init_esc();
 Wire.begin();
Serial.print("HI");
 mpu6050.begin();
 mpu6050.calcGyroOffsets(true);

 const double start_angx = mpu6050.getAngleX();
 const double start_angy = mpu6050.getAngleY();
 delay(1000);
  var = 40;
  less_angx = start_angx - var;
  more_angx = start_angx + var;
  less_angy = start_angy - var;
  more_angy = start_angy + var;
}


void loop() {  
  Serial.println("Enter in a motor ");
  while (Serial.available() == 0) {}
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
//thrust = 254;
// analogWrite(fl, thrust);
// analogWrite(fr, thrust);
// analogWrite(bl, thrust);
// analogWrite(br, thrust);
// delay(1000);
 
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
