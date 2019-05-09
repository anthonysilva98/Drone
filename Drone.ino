#include <MPU6050_tockn.h>
#include <Wire.h>

MPU6050 mpu6050(Wire);

int fl = 3;
int fr = 6;
int bl = 10;
int br = 11;
int thrust, fl_thrust, fr_thrust, bl_thrust, br_thrust;
// byte thrust, fl_thrust, fr_thrust, bl_thrust, br_thrust;  // Byte from -128 -> 127. Total is 255. May be better to use.
double angx, angy, less_angx, less_angy, more_angx, more_angy, start_angx, start_angy, variance;
int timer = 0;
int desired_height = 12;
int trigPin = 4;
int echoPin = 5;
int thrust_change = 10;

void setup()
{
  Serial.begin(9600);
  Wire.begin();

  init_esc();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  double start_angx = mpu6050.getAngleX();
  double start_angy = mpu6050.getAngleY();
  fl_thrust = 50;
  fr_thrust = 50;
  br_thrust = 50;
  bl_thrust = 50;

  set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  variance = 1.0;
  less_angx = start_angx - variance;
  more_angx = start_angx + variance;
}

int availableMemory()
{
  // Use 1024 with ATmega168
  int size = 2048;
  byte *buf;
  while ((buf = (byte *)malloc(--size)) == NULL)
    ;
  free(buf);
  return size;
}

void loop()
{
  Serial.print("Available Memory: ");
  Serial.println(availableMemory());

  mpu6050.update();

  //  if (millis() - timer > 1000) {
  angx = mpu6050.getAngleX();
  angy = mpu6050.getAngleY();
  // delay(200);
  //  }
  int var_height = height();

  if (var_height < desired_height)
    inc_thrust();
  if (var_height > desired_height)
    dec_thrust();

  if (angy < less_angy)
  {
    Serial.println("Rotating Right" + String(angy));
    fr_thrust -= thrust_change;
    br_thrust -= thrust_change;
    set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  }

  if (angy > more_angy)
  {
    Serial.println("Rotating Left" + String(angy));
    fl_thrust -= thrust_change;
    bl_thrust -= thrust_change;
    set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  }

  if (angx > less_angx)
  {
    Serial.println("Rotating Forwards" + String(angx));

    fr_thrust -= thrust_change;
    fl_thrust -= thrust_change;
    set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  }
  if (angx < more_angx)
  {
    Serial.println("Rotating Backwards" + String(angy));
    br_thrust -= thrust_change;
    bl_thrust -= thrust_change;
    set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  }

  set_thrust(fl_thrust, fr_thrust, bl_thrust, br_thrust);
  timer = millis();
}

void set_thrust(int fl, int fr, int bl, int br)
{
  analogWrite(fl, fl_thrust);
  analogWrite(fr, fr_thrust);
  analogWrite(bl, bl_thrust);
  analogWrite(br, br_thrust);
  //  return(1);
}

void init_esc()
{
  Serial.println("Connect Battery.");
  delay(3000);  // Allow time for user to connect battery.
  thrust = 254; // Program max ESC value.
  set_thrust(thrust, thrust, thrust, thrust);
  Serial.println("Max thrust programmed.");
  delay(2000); // Allow delay for ESC to process, may be shortened. Testing needed.

  thrust = 1; // Program min ESC value.
  set_thrust(thrust, thrust, thrust, thrust);
  Serial.println("Min thrust programmed.");
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
  Serial.print(inches);
  Serial.print("in, ");
  Serial.println();

  return (inches);
}

void inc_thrust()
{
  if (is_thrust_max())
  {
    Serial.println("Increasing All Thrust");
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
    Serial.println("Decreasing All Thrust");
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
