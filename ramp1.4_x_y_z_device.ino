#include <AccelStepper.h>

#define X_EN 38
#define X_STEP A0
#define X_DIR A1

AccelStepper stepper_x(AccelStepper::DRIVER, X_STEP, X_DIR);



