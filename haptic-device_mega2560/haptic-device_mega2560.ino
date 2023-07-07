#include <AccelStepper.h>
#include <elapsedMillis.h>
# define y_Enable 56
# define y_Step 60
# define y_Dir 61
# define y_stop 3
# define x_Enable 38
# define x_Step 54
# define x_Dir 55
# define x_stop 2
# define z_Enable 62
# define z_Step 46
# define z_Dir 48


AccelStepper myStepper_y(AccelStepper::DRIVER, y_Step, y_Dir, true); 
AccelStepper myStepper_x(AccelStepper::DRIVER, x_Step, x_Dir, true); 
AccelStepper myStepper_z(AccelStepper::DRIVER, z_Step, z_Dir, true); 
elapsedMillis printTime;


void setup(){
    Serial.begin(115200);
    myStepper_x.setMaxSpeed(2000.0);   // the motor accelerates to this speed exactly without overshoot. 
    myStepper_y.setMaxSpeed(2000.0);   // the motor accelerates to this speed exactly without overshoot. 
    myStepper_z.setMaxSpeed(2000.0);   // the motor accelerates to this speed exactly without overshoot. 
    pinMode(y_Enable, OUTPUT);    // the motor enable pin needs to be LOW to make driver works
    digitalWrite(y_Enable, LOW);
    pinMode(x_Enable, OUTPUT);
    digitalWrite(x_Enable, LOW);
    pinMode(z_Enable, OUTPUT);
    digitalWrite(z_Enable, LOW);
    pinMode(x_stop, INPUT);
    pinMode(y_stop, INPUT);
    go_zero();
}

void loop() {

}

void go_zero(){
  int x_home = 1;
  int y_home = 1;
  int z_home = 1;
  // myStepper_x.setSpeed(-1000);
  myStepper_y.setSpeed(1000);
  while (y_home == 1){
    myStepper_y.runSpeed();
    y_home = digitalRead(y_stop);
  }
  Serial.println("Motors are in zero.");
}