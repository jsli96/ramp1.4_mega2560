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
#define z_Stop 19

bool x_home = false;
bool y_home = false;
bool z_home = false;
const long x_center_point = -22000;
const long y_center_point = -18500;
const long z_center_point = -8000;
long adj_x_center = 0;
long adj_y_center = 0;
long adj_z_center = 0;
long adj_z_center_low = 0;

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
  pinMode(z_Stop, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(z_Stop), z_intr, FALLING); // Stop pin interrupt
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

void z_intr()
{
  myStepper_z.setSpeed(0);
  z_home = true;
  Serial.println("Z Motor speed set to zero.");
}

void setup()
{
  Serial.begin(115200);
  myStepper_x.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_y.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_z.setMaxSpeed(1500.0); // the motor accelerates to this speed exactly without overshoot.
  myStepper_x.setAcceleration(100);
  myStepper_y.setAcceleration(100);
  myStepper_z.setAcceleration(100);
  pin_init();
  initial_home();
  delay(1000);
  Serial.println("Begin moving to the center point.");
  move_to_center(x_center_point, y_center_point, z_center_point);
  Serial.println("System ready!!!");
}

void loop()
{
  if (Serial.available())
  {
    incomingString = Serial.readString();
    if (incomingString == "stop")
    {
      digitalWrite(y_Enable, HIGH);
      digitalWrite(x_Enable, HIGH);
      digitalWrite(z_Enable, HIGH);
      Serial.println("All motors are shutted down!");
    }
    else if (incomingString == "start")
    {
      digitalWrite(y_Enable, LOW);
      digitalWrite(x_Enable, LOW);
      digitalWrite(z_Enable, LOW);
      Serial.println("Motors started!");
    }
    else if (incomingString == "log")
    {
      log_adjust_center_point();
    }
    else
    {
      adjust_probe_position(incomingString);
      points_select(incomingString);
    }

    Serial.println("Standby...");
  }
  delay(1500);
}

void initial_home()
{
  myStepper_y.setSpeed(1200); // Positive speed will move to zero point, negative point will move to oppisite direction.
  myStepper_x.setSpeed(1200);
  myStepper_z.setSpeed(1200);
  while (x_home == false || y_home == false || z_home == false)
  {
    myStepper_y.runSpeed();
    myStepper_x.runSpeed();
    myStepper_z.runSpeed();
  }
  myStepper_x.setCurrentPosition(0);
  myStepper_y.setCurrentPosition(0);
  myStepper_z.setCurrentPosition(0);
  myStepper_x.stop();
  myStepper_y.stop();
  myStepper_z.stop();
  Serial.println("System homed.");
  Serial.print("Current motor poistion (x, y, z): ");
  Serial.print(myStepper_x.currentPosition());
  Serial.print(", ");
  Serial.print(myStepper_y.currentPosition());
  Serial.print(", ");
  Serial.println(myStepper_z.currentPosition());
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
    move_x(incomingString.substring(2).toInt(), 500);
    break;
  case 'y':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_y(incomingString.substring(2).toInt(), 500);
    break;
  case 'z':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_z(incomingString.substring(2).toInt(), 500);
    break;
  }
}

void points_select(String incomingString)
{
  Serial.println(incomingString);
  int temp = incomingString.toInt();
  Serial.print("Incoming point: ");
  Serial.println(temp);
  switch (temp)
  {
  case 24:
    point_24();
    break;
  case 23:
    point_23();
    break;
  case 22:
    point_22();
    break;
  case 21:
    point_21();
    break;
  case 20:
    point_20();
    break;
  case 19:
    point_19();
    break;
  case 18:
    point_18();
    break;
  case 17:
    point_17();
    break;
  case 16:
    point_16();
    break;
  case 15:
    point_15();
    break;
  case 14:
    point_14();
    break;
  case 13:
    point_13();
    break;
  case 12:
    point_12();
    break;
  case 11:
    point_11();
    break;
  case 10:
    point_10();
    break;
  case 9:
    point_9();
    break;
  case 8:
    point_8();
    break;
  case 7:
    point_7();
    break;
  case 6:
    point_6();
    break;
  case 5:
    point_5();
    break;
  case 4:
    point_4();
    break;
  case 3:
    point_3();
    break;
  case 2:
    point_2();
    break;
  case 1:
    point_1();
    break;
  }
}

void move_to_center(long x_c, long y_c, long z_c)
{
  myStepper_x.moveTo(x_c);
  myStepper_y.moveTo(y_c);
  myStepper_z.moveTo(z_c);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0 || myStepper_z.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(x_c, myStepper_x.currentPosition(), 1500));
    myStepper_y.setSpeed(cal_speed_dir(y_c, myStepper_y.currentPosition(), 1500));
    myStepper_z.setSpeed(cal_speed_dir(z_c, myStepper_z.currentPosition(), 1500));
    if (myStepper_x.distanceToGo() != 0)
    {
      myStepper_x.run();
    }
    if (myStepper_y.distanceToGo() != 0)
    {
      myStepper_y.run();
    }
    if (myStepper_z.distanceToGo() != 0)
    {
      myStepper_z.run();
    }
  }
  Serial.print("Current position (x, y, z): ");
  Serial.print(myStepper_x.currentPosition());
  Serial.print(", ");
  Serial.print(myStepper_y.currentPosition());
  Serial.print(", ");
  Serial.print(myStepper_z.currentPosition());
  Serial.print(").");
  Serial.println("Probe at the center position.");
}

void log_adjust_center_point()
{
  adj_x_center = myStepper_x.currentPosition();
  adj_y_center = myStepper_y.currentPosition();
  adj_z_center = myStepper_z.currentPosition();
  Serial.println("Adjust center point logged!");
}

long cal_speed_dir(long target_pos, long current_pos, int abs_speed)
{
  long result = target_pos - current_pos;
  if (result > 0)
  {
    return abs(abs_speed);
  }
  else
  {
    return -abs(abs_speed);
  }
}

void move_x(long distance, int speed)
{
  myStepper_x.move(distance);
  Serial.println(distance);
  if (speed > 1500)
  {
    speed = 1500;
  }
  else if (speed < 0)
  {
    speed = 0;
  }

  while (myStepper_x.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + distance, myStepper_x.currentPosition(), speed));
    myStepper_x.run();
  }
  Serial.print("X current position: ");
  Serial.println(myStepper_x.currentPosition());
}

void move_y(long distance, int speed)
{
  myStepper_y.move(distance);
  Serial.println(distance);
  if (speed > 1500)
  {
    speed = 1500;
  }
  else if (speed < 0)
  {
    speed = 0;
  }
  while (myStepper_y.distanceToGo() != 0)
  {
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + distance, myStepper_y.currentPosition(), speed));
    myStepper_y.run();
  }
  Serial.print("Y current position: ");
  Serial.println(myStepper_y.currentPosition());
}

void move_z(long distance, int speed)
{
  myStepper_z.move(distance);
  Serial.println(distance);
  if (speed > 1500)
  {
    speed = 1500;
  }
  else if (speed < 0)
  {
    speed = 0;
  }
  while (myStepper_z.distanceToGo() != 0)
  {
    myStepper_z.setSpeed(cal_speed_dir(myStepper_z.currentPosition() + distance, myStepper_z.currentPosition(), speed));
    myStepper_z.run();
  }
  Serial.print("Z current position: ");
  Serial.println(myStepper_z.currentPosition());
}

void move_x_y_parallel(long x_distance, long y_distance, int speed)
{
  myStepper_x.move(x_distance);
  myStepper_y.move(y_distance);
  if (speed > 1500)
  {
    speed = 1500;
  }
  else if (speed < 0)
  {
    speed = 0;
  }
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + x_distance, myStepper_x.currentPosition(), speed));
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + y_distance, myStepper_y.currentPosition(), speed));
    if (myStepper_x.distanceToGo() != 0)
    {
      myStepper_x.run();
    }
    if (myStepper_y.distanceToGo() != 0)
    {
      myStepper_y.run();
    }
  }
}

void point_12()
{
  Serial.println("Now execute point 12.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_y(1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 12 Excuted. standby.");
}

void point_24()
{
  Serial.println("Now execute point 24.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_y(2500, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 24 Excuted. standby.");
}

void point_11()
{
  Serial.println("Now execute point 11.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(625, 1082, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 11 Excuted. standby.");
}

void point_23()
{
  Serial.println("Now execute point 23.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(1250, 2165, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 23 Excuted. standby.");
}

void point_10()
{
  Serial.println("Now execute point 10.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(1082, 625, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 10 Excuted. standby.");
}

void point_22()
{
  Serial.println("Now execute point 22.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(2165, 1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 22 Excuted. standby.");
}

void point_9()
{
  Serial.println("Now execute point 9.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x(1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 9 Excuted. standby.");
}

void point_21()
{
  Serial.println("Now execute point 21.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x(2500, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 21 Excuted. standby.");
}

void point_8()
{
  Serial.println("Now execute point 8.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(1082, -625, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 8 Excuted. standby.");
}
void point_20()
{
  Serial.println("Now execute point 20.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(2165, -1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 20 Excuted. standby.");
}
void point_7()
{
  Serial.println("Now execute point 7.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(625, -1082, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 7 Excuted. standby.");
}
void point_19()
{
  Serial.println("Now execute point 19.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(1250, -2165, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 19 Excuted. standby.");
}
void point_6()
{
  Serial.println("Now execute point 6.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_y(-2500, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 6 Excuted. standby.");
}
void point_18()
{
  Serial.println("Now execute point 18.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_y(-1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 18 Excuted. standby.");
}
void point_5()
{
  Serial.println("Now execute point 5.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-625, -1082, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 5 Excuted. standby.");
}
void point_17()
{
  Serial.println("Now execute point 17.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-1250, -2165, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 17 Excuted. standby.");
}
void point_4()
{
  Serial.println("Now execute point 4.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-1082, -625, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 4 Excuted. standby.");
}
void point_16()
{
  Serial.println("Now execute point 16.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-2165, -1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 16 Excuted. standby.");
}
void point_3()
{
  Serial.println("Now execute point 3.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x(-1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 3 Excuted. standby.");
}
void point_15()
{
  Serial.println("Now execute point 15.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x(-2500, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 15 Excuted. standby.");
}
void point_2()
{
  Serial.println("Now execute point 2.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-1082, 625, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 2 Excuted. standby.");
}
void point_14()
{
  Serial.println("Now execute point 14.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-2165, 1250, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 14 Excuted. standby.");
}
void point_1()
{
  Serial.println("Now execute point 1.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-625, 1082, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 1 Excuted. standby.");
}
void point_13()
{
  Serial.println("Now execute point 13.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  delay(500);
  move_x_y_parallel(-1250, 2165, 1400);
  move_z(-2500, 1400);
  delay(2000);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 13 Excuted. standby.");
}
