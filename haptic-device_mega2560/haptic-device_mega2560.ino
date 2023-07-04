#include <AccelStepper.h>
#include <elapsedMillis.h>
# define y_Enable 56
# define y_Step 60
# define y_Dir 61
# define x_Enable 38
# define x_Step 54
# define x_Dir 55
# define z_Enable 62
# define z_Step 46
# define z_Dir 48

AccelStepper myStepper_y(AccelStepper::DRIVER, y_Step, y_Dir, true); 
AccelStepper myStepper_x(AccelStepper::DRIVER, x_Step, x_Dir, true); 
AccelStepper myStepper_z(AccelStepper::DRIVER, z_Step, z_Dir, true); 
elapsedMillis printTime;


void setup(){
    Serial.begin(9600);
    myStepper_y.setMaxSpeed(2000.0);   // the motor accelerates to this speed exactly without overshoot. Try other values.
    myStepper_y.setSpeed(-1000);
    myStepper_z.setMaxSpeed(2000.0);   // the motor accelerates to this speed exactly without overshoot. Try other values.
    myStepper_z.setSpeed(1000);
    pinMode(y_Enable, OUTPUT);    // the motor enable pin needs to be LOW to make driver works
    digitalWrite(y_Enable, LOW);
    pinMode(x_Enable, OUTPUT);
    digitalWrite(x_Enable, LOW);
    pinMode(z_Enable, OUTPUT);
    digitalWrite(z_Enable, LOW);
}

void loop() {
  myStepper_z.runSpeed();
  myStepper_y.runSpeed();

}