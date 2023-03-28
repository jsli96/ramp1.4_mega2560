#include <AccelStepper.h>
#define MEASURE_X_EN 24
#define MEASURE_X_STEP 26
#define MEASURE_X_DIR 28

// #define MEASURE_Y_EN 30
// #define MEASURE_Y_STEP 36
// #define MEASURE_Y_DIR 34

String inString = ""; // string to hold input
int targetPos;
AccelStepper myStepper(AccelStepper::DRIVER, MEASURE_X_STEP, MEASURE_X_DIR);

void setup()
{
  Serial.begin(9600);
  myStepper.enableOutputs();
  myStepper.setMaxSpeed(10000);
  myStepper.setSpeed(10000);
  myStepper.setAcceleration(10000);
  pinMode(MEASURE_X_EN, OUTPUT);
  digitalWrite(MEASURE_X_EN, HIGH);
}

void loop()
{
  if (Serial.available() > 0)
  {
    int inChar = Serial.read();
    if (isDigit(inChar))
    {
      // convert the incoming byte to a char and add it to the string:
      inString += (char)inChar;
    }

    if (inChar == '\n')
    {
      Serial.print("Value:");
      targetPos =inString.toInt();
      Serial.println(targetPos);
      // clear the string for new input:
      inString = "";
    }
      myStepper.moveTo(targetPos);
    while (myStepper.currentPosition() != targetPos){
      myStepper.run();
    }
  }


}
