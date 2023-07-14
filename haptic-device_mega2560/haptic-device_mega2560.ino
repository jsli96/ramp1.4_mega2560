#include <TimerOne.h>
#include <AccelStepper.h>
#include <elapsedMillis.h>
#define y_Enable 56
#define y_Step 60
#define y_Dir 61
#define y_Stop 3
#define x_Enable 38
#define x_Step 54
#define x_Dir 55
#define x_Stop 2
#define z_Enable 62
#define z_Step 46
#define z_Dir 48

bool x_home = false;
bool y_home = false;
bool z_home = false;
const int x_center_point = -25000;
const int y_center_point = -18500;
const int z_center_point = -100;
String incomingString = "0";

AccelStepper myStepper_x(AccelStepper::DRIVER, x_Step, x_Dir, true);
AccelStepper myStepper_y(AccelStepper::DRIVER, y_Step, y_Dir, true);
AccelStepper myStepper_z(AccelStepper::DRIVER, z_Step, z_Dir, true);
elapsedMillis printTime;

void pin_init()
{
  pinMode(y_Enable, OUTPUT);
  digitalWrite(y_Enable, LOW); // the motor enable pin needs to be LOW to make driver works
  pinMode(x_Enable, OUTPUT);
  digitalWrite(x_Enable, LOW); // the motor enable pin needs to be LOW to make driver works
  pinMode(z_Enable, OUTPUT);
  digitalWrite(z_Enable, LOW); // the motor enable pin needs to be LOW to make driver works
  pinMode(x_Stop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(x_Stop), x_intr, FALLING); // Stop pin interrupt
  pinMode(y_Stop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(y_Stop), y_intr, FALLING); // Stop pin interrupt
}

void initial_home()
{
  while (x_home == false || y_home == false)
  {
    myStepper_y.runSpeed();
    myStepper_x.runSpeed();
  }
  myStepper_x.setCurrentPosition(0);
  myStepper_y.setCurrentPosition(0);
  myStepper_x.stop();
  myStepper_y.stop();
  Serial.println("System homed.");
  Serial.print("Current motor poistion (x, y): ");
  Serial.print(myStepper_x.currentPosition());
  Serial.print(", ");
  Serial.println(myStepper_y.currentPosition());
}

void x_intr()
{
  myStepper_x.setSpeed(0);
  x_home = true;
  Serial.println("X Motor speed set to zero.");
}

void y_intr()
{
  myStepper_y.setSpeed(0);
  y_home = true;
  Serial.println("Y Motor speed set to zero.");
}

void setup()
{
  Serial.begin(115200);
  myStepper_x.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_y.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_z.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_x.setAcceleration(100);
  myStepper_y.setAcceleration(100);
  myStepper_y.setSpeed(1200); // Positive speed will move to zero point, negative point will move to oppisite direction.
  myStepper_x.setSpeed(1200);
  pin_init();
  // initial_home();
  // delay(1000);
  // Serial.println("Begin moving to the center point.");
  // move_to_center();
  // Serial.println("System ready!!!");
}

void loop()
{
  if (Serial.available())
  {
    incomingString = Serial.readString();
    if (incomingString == "stop" ){
        digitalWrite(y_Enable, HIGH);
        digitalWrite(x_Enable, HIGH);
        digitalWrite(z_Enable, HIGH);
        Serial.println("All motors are shutted down!");
    }
    else{
      adjust_probe_position(incomingString);
    }
  }
  Serial.println("Standby...");
  delay(1000);
}

void adjust_probe_position(String incomingString)
{
  switch (incomingString[0])
  {
  case 'x':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_x(incomingString.substring(2).toInt());
    break;
  case 'y':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_y(incomingString.substring(2).toInt());
    break;
  case 'z':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_z(incomingString.substring(2).toInt());
    break;
  }
}

void move_to_center()
{
  myStepper_x.moveTo(x_center_point);
  myStepper_y.moveTo(y_center_point);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(x_center_point, myStepper_x.currentPosition(), 1500));
    myStepper_y.setSpeed(cal_speed_dir(y_center_point, myStepper_y.currentPosition(), 1500));
    if (myStepper_x.distanceToGo() != 0)
    {
      myStepper_x.run();
      // Serial.print("Distance to go on x direction: ");
      // Serial.println(myStepper_x.distanceToGo());
    }
    if (myStepper_y.distanceToGo() != 0)
    {
      myStepper_y.run();
      // Serial.print("Distance to go on y direction: ");
      //  Serial.println(myStepper_y.distanceToGo());
    }
  }
  Serial.print("Current position (x, y): (");
  Serial.print(myStepper_x.currentPosition());
  Serial.print(", ");
  Serial.print(myStepper_y.currentPosition());
  Serial.print(").");
  Serial.println("Probe at the center position.");
}

int cal_speed_dir(int target_pos, int current_pos, int abs_speed)
{
  int result = target_pos - current_pos;
  if (result > 0)
  {
    return abs(abs_speed);
  }
  else
  {
    return -abs(abs_speed);
  }
}

void move_x(int distance)
{
  myStepper_x.move(distance);
  Serial.println(distance);
  while (myStepper_x.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + distance, myStepper_x.currentPosition(), 500));
    myStepper_x.run();
  }
  Serial.print("X current position: ");
  Serial.println(myStepper_x.currentPosition());
}

void move_y(int distance)
{
  myStepper_y.move(distance);
  Serial.println(distance);
  while (myStepper_y.distanceToGo() != 0)
  {
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + distance, myStepper_y.currentPosition(), 500));
    myStepper_y.run();
  }
  Serial.print("Y current position: ");
  Serial.println(myStepper_x.currentPosition());
}

void move_z(int distance)
{
  myStepper_z.move(distance);
  Serial.println(distance);
  while (myStepper_z.distanceToGo() != 0)
  {
    myStepper_z.setSpeed(cal_speed_dir(myStepper_z.currentPosition() + distance, myStepper_z.currentPosition(), 500));
    myStepper_z.run();
  }
  Serial.print("Z current position: ");
  Serial.println(myStepper_z.currentPosition());
}