#include <AccelStepper.h>

#define X_EN 38
#define X_STEP A0
#define X_DIR A1
#define Y_EN A2
#define Y_STEP A6
#define Y_DIR A7
#define Z_EN A8
#define Z_STEP 46
#define Z_DIR 48

AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP, X_DIR);
AccelStepper stepper_y(AccelStepper::DRIVER, Y_STEP, Y_DIR);
AccelStepper stepper_z(AccelStepper::DRIVER, Z_STEP, Z_DIR);
void setup(){
    Serial.begin(9600);
    pinMode(X_EN, OUTPUT);
    digitalWrite(X_EN, LOW);
    stepper_x.setMaxSpeed(2500);
    stepper_x.setSpeed(2500);
    stepper_x.setAcceleration(5000);
    stepper_x.moveTo(10000);
    pinMode(Y_EN, OUTPUT);
    digitalWrite(Y_EN, LOW);
    stepper_y.setMaxSpeed(2500);
    //stepper_y.setSpeed(500);
    stepper_y.setAcceleration(10000);
    stepper_y.moveTo(10000);
    pinMode(Z_EN, OUTPUT);
    digitalWrite(Z_EN, HIGH);
    stepper_z.setMaxSpeed(2500);
    stepper_z.setSpeed(2500);
    stepper_z.setAcceleration(10000);
    stepper_z.moveTo(-3000);
}


void loop(){
//    int tmp = stepper_x.distanceToGo();
//    Serial.println(tmp);
    stepper_x.run();
    stepper_y.run();
    stepper_z.run();
}
