// Setting up the MPU6050
// SDA -> A4
// SCL -> A5
// INT -> 2

#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

int buttonIn = 7;
int fl = 3;
int fr = 6;
int bl = 10;
int br = 11;
int standby = 12;
int trigPin = 4;
int echoPin = 5;

int timer = 0;
int thrust, fl_thrust, fr_thrust, bl_thrust, br_thrust = 0;
// byte thrust, fl_thrust, fr_thrust, bl_thrust, br_thrust;  // Byte from -128 -> 127. Total is 255. May be better to use.
double angx, angy, less_angx, less_angy, more_angx, more_angy, start_angx, start_angy, variance;
int thrust_change = 10;
int desired_height = 12;


void setup()
{
    
  Wire.begin();
pinMode(buttonIn, INPUT);
pinMode(standby, OUTPUT);
pinMode(echoPin, INPUT);
pinMode(trigPin, OUTPUT);

  while (!digitalRead(buttonIn))
  {  // Flash standby LED until button press.
      digitalWrite(standby, 1);
      delay(250);
      digitalWrite(standby, 0);
      delay(250);
  }

  init_esc();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  double start_angx = mpu6050.getAngleX();
  double start_angy = mpu6050.getAngleY();
  variance = 1.0;
  less_angx = start_angx - variance;
  more_angx = start_angx + variance;
}


void loop()
{
  mpu6050.update();

  angx = mpu6050.getAngleX();
  angy = mpu6050.getAngleY();
  int var_height = height();

  if (var_height < desired_height)  // Too low, more power.
    inc_thrust();

  if (var_height > desired_height)  // Too high, less power.
    dec_thrust();

  if (angy < less_angy)  // Too left, rotate right.
  {
    fr_thrust -= thrust_change;
    br_thrust -= thrust_change;
  }

  if (angy > more_angy)  // Too right, rotate left.
  {
    fl_thrust -= thrust_change;
    bl_thrust -= thrust_change;
  }

  if (angx > less_angx)  // Too backwards, rotate forwards.
  {
    fr_thrust -= thrust_change;
    fl_thrust -= thrust_change;
  }
  if (angx < more_angx)  // Too forwards, rotate backwards.
  {
    br_thrust -= thrust_change;
    bl_thrust -= thrust_change;
  }
  // All adjustments made, send adjusted values to motors.
  set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
}

void set_thrust(int fl_thrust, int fr_thrust, int bl_thrust, int br_thrust)  
{// Method to set all motor values at once.
  analogWrite(fl, fl_thrust);
  analogWrite(fr, fr_thrust);
  analogWrite(bl, bl_thrust);
  analogWrite(br, br_thrust);
}

void init_esc()
{
  delay(1000);  // Allow time for user to connect battery.
  thrust = 254; // Program max ESC value.
  set_thrust(thrust, thrust, thrust, thrust);
  delay(1000); // Allow delay for ESC to process, may be shortened. Testing needed.

  thrust = 1; // Program min ESC value.
  set_thrust(thrust, thrust, thrust, thrust);
  delay(1000);
}

int height()
{
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);

  // double cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  double inches = (duration / 2) / 74; // Divide by 74 or multiply by 0.0135
  return (inches);
}

void inc_thrust()
{
  if (is_thrust_max())
  {
    fl_thrust += thrust_change;
    fr_thrust += thrust_change;
    bl_thrust += thrust_change;
    br_thrust += thrust_change;
  }
}

void dec_thrust()
{
  if (is_thrust_min())
  {
    fl_thrust -= thrust_change;
    fr_thrust -= thrust_change;
    bl_thrust -= thrust_change;
    br_thrust -= thrust_change;
  }
}

bool is_thrust_max()
{
  return (fl_thrust < 255 && fr_thrust < 255 && bl_thrust < 255 && br_thrust < 255);
}

bool is_thrust_min()
{
  return (fl_thrust > 0 && fr_thrust > 0 && bl_thrust > 0 && br_thrust > 0);
}
