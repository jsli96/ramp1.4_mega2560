// One tooth is 2mm
// Gear has 16 teeth.
// Stepper one rev 200*16=3200 pause
// 320 mm , 40mm
// middle point is 160 mm
// 160 /(16*2) = 5 rev
// 5 * 3200 = 16000 pause
#include <AccelStepper.h>
#define MEASURE_X_EN 24
#define MEASURE_X_STEP 26
#define MEASURE_X_DIR 28
#define X_STOP_PIN 15
#define MEASURE_Y_EN 30
#define MEASURE_Y_STEP 36
#define MEASURE_Y_DIR 34
#define Y_STOP_PIN 14

AccelStepper stepper_measure_x(AccelStepper::DRIVER, MEASURE_X_STEP, MEASURE_X_DIR);
AccelStepper stepper_measure_y(AccelStepper::DRIVER, MEASURE_Y_STEP, MEASURE_Y_DIR);

void setup()
{
  Serial.begin(9600);
  motor_parameters_init();
motor_initial_calibration();
  motor_center_position();
}

void loop()
{
}

void motor_parameters_init()
{
  stepper_measure_x.setMaxSpeed(20000);
  stepper_measure_x.setSpeed(20000);
  stepper_measure_x.setAcceleration(20000);
  stepper_measure_y.setMaxSpeed(20000);
  stepper_measure_y.setSpeed(20000);
  stepper_measure_y.setAcceleration(20000);
  pinMode(MEASURE_X_EN, OUTPUT);
  digitalWrite(MEASURE_X_EN, HIGH);
  pinMode(MEASURE_Y_EN, OUTPUT);
  digitalWrite(MEASURE_Y_EN, HIGH);
  pinMode(X_STOP_PIN, INPUT);
  pinMode(Y_STOP_PIN, INPUT);
}

void motor_initial_calibration()
{
  Serial.print("Start calibration process.\n");
  int home_x = digitalRead(X_STOP_PIN);
  int home_y = digitalRead(Y_STOP_PIN);
  while (home_x != 0)
  {
    stepper_measure_x.move(10);
    stepper_measure_x.run();
    home_x = digitalRead(X_STOP_PIN);
    if (home_x == 0)
    {
      Serial.println("Measuring device arrive X-axis home position.\n");
      stepper_measure_x.setCurrentPosition(0);
      Serial.println("Measuring device X-axis position has been reset to zero.\n");
    }
  }

  while (home_y != 0)
  {
    stepper_measure_y.move(10);
    stepper_measure_y.run();
    home_y = digitalRead(Y_STOP_PIN);
    if (home_y == 0)
    {
      Serial.println("Measuring device arrive Y-axis home position.\n");
      stepper_measure_y.setCurrentPosition(0);
      Serial.println("Measuring device Y-axis position has been reset to zero.\n");
    }
  }
  Serial.println("Initial Calibration complete!\n");
}

void motor_center_position()
{
  stepper_measure_x.moveTo(-16000);
  stepper_measure_y.moveTo(-16000);
  while (stepper_measure_x.distanceToGo() != 0 || stepper_measure_y.distanceToGo() != 0){
    stepper_measure_x.run();
    stepper_measure_y.run();
  }
  Serial.println("Measuring system ready to use!\n");
}