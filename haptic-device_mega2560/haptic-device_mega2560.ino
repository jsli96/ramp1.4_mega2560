#include <AccelStepper.h>
#include <elapsedMillis.h>
# define y_Enable A2
# define y_Step A6
# define y_Dir A7
# define x_Enable 38
# define x_Step A0
# define x_Dir A1

AccelStepper myStepper_y(AccelStepper::DRIVER, y_Step, y_Dir, true); 
AccelStepper myStepper_x(AccelStepper::DRIVER, x_Step, x_Dir, true); 
elapsedMillis printTime;


void setup(){
    Serial.begin(9600);
    myStepper_x.setMaxSpeed(1000.0);   // the motor accelerates to this speed exactly without overshoot. Try other values.
    myStepper_x.setSpeed(200);
    pinMode(y_Enable, OUTPUT);
    digitalWrite(y_Enable, HIGH);
    pinMode(x_Enable, OUTPUT);
    digitalWrite(x_Enable, HIGH);
}

void loop() {
  myStepper_x.runSpeed();
}