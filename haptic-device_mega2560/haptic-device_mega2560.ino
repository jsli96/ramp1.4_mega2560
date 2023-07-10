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

AccelStepper myStepper_x(AccelStepper::DRIVER, x_Step, x_Dir, true);
AccelStepper myStepper_y(AccelStepper::DRIVER, y_Step, y_Dir, true);
AccelStepper myStepper_z(AccelStepper::DRIVER, z_Step, z_Dir, true);
elapsedMillis printTime;

void pin_setup(){
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

void setup()
{
  Serial.begin(115200);
  myStepper_x.setMaxSpeed(2000.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_y.setMaxSpeed(2000.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_z.setMaxSpeed(2000.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_y.setSpeed(-1000);
  myStepper_x.setSpeed(-1000);
  pin_setup();
  initial_home();
}

void loop()
{
    myStepper_y.runSpeed();
    myStepper_x.runSpeed();

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