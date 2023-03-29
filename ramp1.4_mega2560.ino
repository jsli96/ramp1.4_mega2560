#include <AccelStepper.h>
#define MEASURE_X_EN 24
#define MEASURE_X_STEP 26
#define MEASURE_X_DIR 28
#define X_STOP_PIN 15
#define MEASURE_Y_EN 30
#define MEASURE_Y_STEP 36
#define MEASURE_Y_DIR 34
#define Y_STOP_PIN 14


AccelStepper myStepper(AccelStepper::DRIVER, MEASURE_X_STEP, MEASURE_X_DIR);
String inString = ""; // string to hold input
int targetPos;


void setup()
{
  Serial.begin(9600);
  myStepper.enableOutputs();
  myStepper.setMaxSpeed(20000);
  myStepper.setSpeed(20000);
  myStepper.setAcceleration(20000);
  pinMode(MEASURE_X_EN, OUTPUT);
  digitalWrite(MEASURE_X_EN, HIGH);
  pinMode(MEASURE_Y_EN, OUTPUT);
  digitalWrite(MEASURE_Y_EN, HIGH);
  pinMode(X_STOP_PIN,INPUT);
  pinMode(Y_STOP_PIN,INPUT);
  initial_calibration();
}

void loop()
{
  // if (Serial.available() > 0)
  // {
  //   int inChar = Serial.read();
  //   if (isDigit(inChar))
  //   {
  //     // convert the incoming byte to a char and add it to the string:
  //     inString += (char)inChar;
  //   }

  //   if (inChar == '\n')
  //   {
  //     Serial.print("Value:");
  //     targetPos = inString.toInt();
  //     Serial.println(targetPos);
  //     // clear the string for new input:
  //     inString = "";
  //   }
  //   myStepper.moveTo(targetPos);
  //   while (myStepper.currentPosition() != targetPos)
  //   {
  //     myStepper.run();
  //   }
  // }
}


void initial_calibration(){
  int home_x = digitalRead(X_STOP_PIN);
  int home_y = digitalRead(Y_STOP_PIN);
  while (home_x != 0 ){
    myStepper.move(1);
    myStepper.run();
    home_x = digitalRead(X_STOP_PIN);
  }
  if (home_x == 0){
    Serial.println("Measuring device arrive X-axis home position.");
    myStepper.setCurrentPosition(0);
    Serial.println("Measuring device X-axis position has been reset to zero.");
  }
}