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
#define vib_motor_1 16

bool x_home = false;
bool y_home = false;
bool z_home = false;
const long x_center_point = -12000;
const long y_center_point = -12000;
const long z_center_point = -1000;
const int full_distance = 5000;
const int half_distance = 2250;
const int full_distance_at_45 = 3535;
const int half_distance_at_45 = 1590;
long adj_x_center = -12000;
long adj_y_center = -12000;
long adj_z_center = -1000;
long adj_z_center_low = -1000;
long adj_z_left_low = -1000;
long adj_z_right_low = -1000;
long adj_z_front_low = -1000;
long adj_z_back_low = -1000;
long z_center_move_distance = 0;
long z_left_move_distance = 0;
long z_right_move_distance = 0;
long z_front_move_distance = 0;
long z_back_move_distance = 0;
boolean z_calibration = false;
boolean vib_mode = false;
boolean stretch_mode = false;
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
  pinMode(vib_motor_1, OUTPUT);
  digitalWrite(vib_motor_1, LOW);
  // Initial the motor and return to the zero.
  initial_to_zero(); // Find the zero position.
}

void x_intr()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 900)
  {
    myStepper_x.setSpeed(0);
    x_home = true;
    Serial.println("X Motor speed set to zero.");
  }
  last_interrupt_time = interrupt_time;
}

void y_intr()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 900)
  {
    myStepper_y.setSpeed(0);
    y_home = true;
    Serial.println("Y Motor speed set to zero.");
  }
  last_interrupt_time = interrupt_time;
}

void z_intr()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 600)
  {
    myStepper_z.setSpeed(0);
    z_home = true;
    Serial.println("Z Motor speed set to zero.");
  }
  last_interrupt_time = interrupt_time;
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
  pin_init(); // Initial the pin output functionalities.
  delay(1000);
  Serial.println("Begin moving to the center point.");
  move_to_center(x_center_point, y_center_point, z_center_point); // Move to the pre-set center point.
  detachInterrupt(digitalPinToInterrupt(x_Stop));
  detachInterrupt(digitalPinToInterrupt(y_Stop));
  detachInterrupt(digitalPinToInterrupt(z_Stop));
  Serial.println("System ready!!!");
}

void loop()
{
  if (Serial.available())
  {
    incomingString = Serial.readString();
    if (incomingString == "stop motors")
    {
      digitalWrite(y_Enable, HIGH);
      digitalWrite(x_Enable, HIGH);
      digitalWrite(z_Enable, HIGH);
      Serial.println("All motors are shutted down!");
    }
    else if (incomingString == "start motors")
    {
      digitalWrite(y_Enable, LOW);
      digitalWrite(x_Enable, LOW);
      digitalWrite(z_Enable, LOW);
      Serial.println("Motors started!");
    }
    else if (incomingString == "log xyz")
    {
      log_adjust_center_point();
    }
    else if (incomingString == "log center z lowest point")
    {
      log_center_z_lowest_point();
    }
    else if (incomingString == "log left z lowest point")
    {
      log_left_z_lowest_point();
    }
    else if (incomingString == "log right z lowest point")
    {
      log_right_z_lowest_point();
    }
    else if (incomingString == "log front z lowest point")
    {
      log_front_z_lowest_point();
    }
    else if (incomingString == "log back z lowest point")
    {
      log_back_z_lowest_point();
    }
    else if (incomingString == "pre-set home")
    {
      move_to_center(x_center_point, y_center_point, z_center_point);
    }
    else if (incomingString == "adjusted home")
    {
      move_to_center(adj_x_center, adj_y_center, adj_z_center);
    }
    else if (incomingString == "show current location")
    {
      show_current_location();
    }
    else if (incomingString[0] == 'x' || incomingString[0] == 'y' || incomingString[0] == 'z')
    {
      adjust_probe_position(incomingString);
    }
    else
    {
      points_select(incomingString);
    }

    Serial.println("Standby...");
  }
  delay(500);
}

void initial_to_zero()
{
  myStepper_y.setSpeed(1250); // Positive speed will move to zero point, negative point will move to oppisite direction.
  myStepper_x.setSpeed(1250);
  myStepper_z.setSpeed(1250);
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
  Serial.println("Motor at zero position.");
}

void move_to_center(long x_c, long y_c, long z_c)
{
  myStepper_x.moveTo(x_c);
  myStepper_y.moveTo(y_c);
  myStepper_z.moveTo(z_c);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0 || myStepper_z.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(x_c, myStepper_x.currentPosition(), 1400));
    myStepper_y.setSpeed(cal_speed_dir(y_c, myStepper_y.currentPosition(), 1400));
    myStepper_z.setSpeed(cal_speed_dir(z_c, myStepper_z.currentPosition(), 1400));
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
  Serial.println("Probe at the center position.");
  show_current_location();
}

void show_current_location()
{
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
    move_x(incomingString.substring(2).toInt(), 1400);
    break;
  case 'y':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_y(incomingString.substring(2).toInt(), 1400);
    break;
  case 'z':
    Serial.print("Axis: ");
    Serial.println(incomingString[0]);
    Serial.print("Destination: ");
    Serial.println(incomingString.substring(2).toInt());
    move_z(incomingString.substring(2).toInt(), 1400);
    break;
  }
}

void points_select(String incomingString)
{
  Serial.println(incomingString);
  if (adj_x_center == -12000 || adj_y_center == -12000)
  {
    Serial.println("Please calbiate and log x y z postion first!");
  }
  if (z_calibration == false)
  {
    Serial.println("Please calbiate the lowest z postion first!");
  }
  if (z_calibration == true && adj_x_center != -12000 && adj_y_center != -12000)
  {
    int temp = incomingString.toInt();
    Serial.print("Incoming point: ");
    Serial.println(temp);
    switch (temp)
    {
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
    case 21:
      h_left();
      break;
    case 22:
      h_right();
      break;
    case 23:
      v_backward();
      break;
    case 24:
      v_forward();
      break;
    case 25:
      multi_fb();
      break;
    case 26:
      multi_incomplete_t();
      break;
    case 27:
      multi_complete_t();
      break;
    case 28:
      big_left_curve();
      break;
    case 29:
      small_right_curve();
      break;
    case 30:
      s_curve();
      break;

      //======================================================================//
    case 46:
      point_16_vib();
      break;
    case 45:
      point_15_vib();
      break;
    case 44:
      point_14_vib();
      break;
    case 43:
      point_13_vib();
      break;
    case 42:
      point_12_vib();
      break;
    case 41:
      point_11_vib();
      break;
    case 40:
      point_10_vib();
      break;
    case 39:
      point_9_vib();
      break;
    case 38:
      point_8_vib();
      break;
    case 37:
      point_7_vib();
      break;
    case 36:
      point_6_vib();
      break;
    case 35:
      point_5_vib();
      break;
    case 34:
      point_4_vib();
      break;
    case 33:
      point_3_vib();
      break;
    case 32:
      point_2_vib();
      break;
    case 31:
      point_1_vib();
      break;
    case 51:
      h_left_vib();
      break;
    case 52:
      h_right_vib();
      break;
    case 53:
      v_backward_vib();
      break;
    case 54:
      v_forward_vib();
      break;
    case 55:
      multi_fb_vib();
      break;
    case 56:
      multi_incomplete_t_vib();
      break;
    case 57:
      multi_complete_t_vib();
      break;
    case 58:
      big_left_curve_vib();
      break;
    case 59:
      small_right_curve_vib();
      break;
    case 60:
      s_curve_vib();
      break;
      //======================================================================//

    default:
      Serial.println("Commands not match!");
      break;
    }
  }
}

void log_adjust_center_point()
{
  adj_x_center = myStepper_x.currentPosition();
  adj_y_center = myStepper_y.currentPosition();
  adj_z_center = myStepper_z.currentPosition();
  Serial.println("Adjust center point logged!");
  show_current_location();
}

void log_center_z_lowest_point()
{
  adj_z_center_low = myStepper_z.currentPosition();
  z_center_move_distance = adj_z_center_low - adj_z_center;
  Serial.println("Z axis center lowest point logged!");
  Serial.print("Z axis center movement distance is ");
  Serial.println(z_center_move_distance);
  Serial.print(z_center_move_distance);
  Serial.print(',');
  Serial.print(z_left_move_distance);
  Serial.print(',');
  Serial.print(z_right_move_distance);
  Serial.print(',');
  Serial.print(z_front_move_distance);
  Serial.print(',');
  Serial.println(z_back_move_distance);

  show_current_location();
  if (z_center_move_distance == 0 || z_left_move_distance == 0 || z_right_move_distance == 0 || z_front_move_distance == 0 || z_back_move_distance == 0)
  {
    z_calibration = false;
  }
  else
  {
    z_calibration = true;
  }
}

void log_left_z_lowest_point()
{
  adj_z_left_low = myStepper_z.currentPosition();
  z_left_move_distance = adj_z_left_low - adj_z_center;
  Serial.println("Z axis left center lowest point logged!");
  Serial.print("Z axis left center movement distance is ");
  Serial.println(z_left_move_distance);
  Serial.print(z_center_move_distance);
  Serial.print(',');
  Serial.print(z_left_move_distance);
  Serial.print(',');
  Serial.print(z_right_move_distance);
  Serial.print(',');
  Serial.print(z_front_move_distance);
  Serial.print(',');
  Serial.println(z_back_move_distance);
  show_current_location();
  if (z_center_move_distance == 0 || z_left_move_distance == 0 || z_right_move_distance == 0 || z_front_move_distance == 0 || z_back_move_distance == 0)
  {
    z_calibration = false;
  }
  else
  {
    z_calibration = true;
  }
}

void log_right_z_lowest_point()
{
  adj_z_right_low = myStepper_z.currentPosition();
  z_right_move_distance = adj_z_right_low - adj_z_center;
  Serial.println("Z axis right lowest point logged!");
  Serial.print("Z axis right movement distance is ");
  Serial.println(z_right_move_distance);
  Serial.print(z_center_move_distance);
  Serial.print(',');
  Serial.print(z_left_move_distance);
  Serial.print(',');
  Serial.print(z_right_move_distance);
  Serial.print(',');
  Serial.print(z_front_move_distance);
  Serial.print(',');
  Serial.println(z_back_move_distance);
  show_current_location();
  if (z_center_move_distance == 0 || z_left_move_distance == 0 || z_right_move_distance == 0 || z_front_move_distance == 0 || z_back_move_distance == 0)
  {
    z_calibration = false;
  }
  else
  {
    z_calibration = true;
  }
}

void log_front_z_lowest_point()
{
  adj_z_front_low = myStepper_z.currentPosition();
  z_front_move_distance = adj_z_front_low - adj_z_center;
  Serial.println("Z axis front lowest point logged!");
  Serial.print("Z axis front movement distance is ");
  Serial.println(z_front_move_distance);
  Serial.print(z_center_move_distance);
  Serial.print(',');
  Serial.print(z_left_move_distance);
  Serial.print(',');
  Serial.print(z_right_move_distance);
  Serial.print(',');
  Serial.print(z_front_move_distance);
  Serial.print(',');
  Serial.println(z_back_move_distance);
  show_current_location();
  if (z_center_move_distance == 0 || z_left_move_distance == 0 || z_right_move_distance == 0 || z_front_move_distance == 0 || z_back_move_distance == 0)
  {
    z_calibration = false;
  }
  else
  {
    z_calibration = true;
  }
}

void log_back_z_lowest_point()
{
  adj_z_back_low = myStepper_z.currentPosition();
  z_back_move_distance = adj_z_back_low - adj_z_center;
  Serial.println("Z axis back lowest point logged!");
  Serial.print("Z axis back movement distance is ");
  Serial.println(z_back_move_distance);
  Serial.print(z_center_move_distance);
  Serial.print(',');
  Serial.print(z_left_move_distance);
  Serial.print(',');
  Serial.print(z_right_move_distance);
  Serial.print(',');
  Serial.print(z_front_move_distance);
  Serial.print(',');
  Serial.println(z_back_move_distance);
  show_current_location();
  if (z_center_move_distance == 0 || z_left_move_distance == 0 || z_right_move_distance == 0 || z_front_move_distance == 0 || z_back_move_distance == 0)
  {
    z_calibration = false;
  }
  else
  {
    z_calibration = true;
  }
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
  // Serial.println(distance);
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
  // Serial.println(distance);
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
  // Serial.println(distance);
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

void move_x_y_z_in_task(long x_distance, long y_distance, long z_distance)
{
  double z_speed;
  myStepper_x.move(x_distance);
  myStepper_y.move(y_distance);
  myStepper_z.move(z_distance);
  if (x_distance == 0)
  {
    z_speed = abs(z_distance / (y_distance / 1400));
  }
  else
  {
    z_speed = abs(z_distance / (x_distance / 1400));
  }
  Serial.println(z_speed);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + x_distance, myStepper_x.currentPosition(), 1400));
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + y_distance, myStepper_y.currentPosition(), 1400));
    myStepper_z.setSpeed(cal_speed_dir(myStepper_z.currentPosition() + z_distance, myStepper_z.currentPosition(), z_speed));

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
}

//=========================Skin stretch patterns=================//
void point_16()
{
  Serial.println("Now execute point 16.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = adj_z_front_low - adj_z_center_low;
  move_x_y_z_in_task(0, full_distance, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 16 Excuted. standby.");
}
void point_8()
{
  Serial.println("Now execute point 8.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_y(half_distance, 700);
  // int z_adjustment = adj_z_front_low - adj_z_center_low;
  // move_x_y_z_in_task(0, half_distance, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 8 Excuted. standby.");
}
void point_1()
{
  Serial.println("Now execute point 1.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(-full_distance_at_45, full_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 1 Excuted. standby.");
}
void point_9()
{
  Serial.println("Now execute point 9.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x_y_parallel(-half_distance_at_45, half_distance_at_45, 700);
  // int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center_low;
  // move_x_y_z_in_task(-half_distance_at_45, half_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 9 Excuted. standby.");
}
void point_2()
{
  Serial.println("Now execute point 10.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = adj_z_right_low - adj_z_center_low;
  move_x_y_z_in_task(-full_distance, 0, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 2 Excuted. standby.");
}
void point_10()
{
  Serial.println("Now execute point 10.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x(-half_distance, 700);
  // int z_adjustment = adj_z_right_low - adj_z_center_low;
  // move_x_y_z_in_task(-half_distance, 0, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 10 Excuted. standby.");
}
void point_3()
{
  Serial.println("Now execute point 3.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_back_low + adj_z_right_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(-full_distance_at_45, -full_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 3 Excuted. standby.");
}
void point_11()
{
  Serial.println("Now execute point 11.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x_y_parallel(-half_distance_at_45, -half_distance_at_45, 700);
  // int z_adjustment = ((adj_z_back_low + adj_z_right_low) / 2) - adj_z_center_low;
  // move_x_y_z_in_task(-half_distance_at_45, -half_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 11 Excuted. standby.");
}
void point_4()
{
  Serial.println("Now execute point 4.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = adj_z_back_low - adj_z_center_low;
  move_x_y_z_in_task(0, -full_distance, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 4 Excuted. standby.");
}
void point_12()
{
  Serial.println("Now execute point 12.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_y(-half_distance, 700);
  // int z_adjustment = adj_z_back_low - adj_z_center_low;
  // move_x_y_z_in_task(0, -half_distance, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 12 Excuted. standby.");
}
void point_5()
{
  Serial.println("Now execute point 5.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_back_low + adj_z_left_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(full_distance_at_45, -full_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 5 Excuted. standby.");
}
void point_13()
{
  Serial.println("Now execute point 13.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x_y_parallel(half_distance_at_45, -half_distance_at_45, 700);
  // int z_adjustment = ((adj_z_back_low + adj_z_left_low) / 2) - adj_z_center_low;
  // move_x_y_z_in_task(half_distance_at_45, -half_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 13 Excuted. standby.");
}
void point_6()
{
  Serial.println("Now execute point 6.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = adj_z_left_low - adj_z_center_low;
  move_x_y_z_in_task(full_distance, 0, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 6 Excuted. standby.");
}
void point_14()
{
  Serial.println("Now execute point 14.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x(half_distance, 700);
  // int z_adjustment = adj_z_left_low - adj_z_center_low;
  // move_x_y_z_in_task(half_distance, 0, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 14 Excuted. standby.");
}
void point_7()
{
  Serial.println("Now execute point 7.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_left_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(full_distance_at_45, full_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 7 Excuted. standby.");
}
void point_15()
{
  Serial.println("Now execute point 15.");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  move_x_y_parallel(half_distance_at_45, half_distance_at_45, 700);
  // int z_adjustment = ((adj_z_front_low + adj_z_left_low) / 2) - adj_z_center_low;
  // move_x_y_z_in_task(half_distance_at_45, half_distance_at_45, z_adjustment);
  delay(1200);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 15 Excuted. standby.");
}
void big_left_curve()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  long start_point_x = adj_x_center;
  long start_point_y = adj_y_center - full_distance;
  move_y(-full_distance, 1400);
  delay(500);
  int z_moveDown_distance = adj_z_back_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  long pre_x_internal_point = adj_x_center;
  long pre_y_internal_point = adj_y_center - full_distance;
  double step = PI / 1000;

  for (double angle = 0; angle < PI; angle = angle + step)
  {
    double x_internal_point_1 = adj_x_center + full_distance * sin(angle);
    double y_internal_point_1 = adj_y_center - full_distance * cos(angle);
    double x_move_distance = x_internal_point_1 - pre_x_internal_point;
    double y_move_distance = y_internal_point_1 - pre_y_internal_point;
    myStepper_x.move(long(x_move_distance));
    myStepper_y.move(long(y_move_distance));
    float x_speed;
    float y_speed;
    x_speed = sqrt(pow(1400, 2) / (1 + pow((y_move_distance / x_move_distance), 2)));
    y_speed = sqrt(pow(1400, 2) / (1 + pow((x_move_distance / y_move_distance), 2)));
    while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
    {
      myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + long(x_move_distance), myStepper_x.currentPosition(), x_speed));
      myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + long(y_move_distance), myStepper_y.currentPosition(), y_speed));
      if (myStepper_x.distanceToGo() != 0)
      {
        myStepper_x.run();
      }
      if (myStepper_y.distanceToGo() != 0)
      {
        myStepper_y.run();
      }
    }
    pre_x_internal_point = x_internal_point_1;
    pre_y_internal_point = y_internal_point_1;
    int current_z_position = myStepper_z.currentPosition();
    if (angle <= (PI / 2))
    {
      if (abs(current_z_position - adj_z_left_low) > 30)
      {
        if (current_z_position < adj_z_left_low)
        {
          move_z(10, 1400);
        }
        else if (current_z_position > adj_z_left_low)
        {
          move_z(-10, 1400);
        }
      }
    }
    else if (angle > (PI / 2))
    {
      if (abs(current_z_position - adj_z_front_low) > 30)
      {
        if (current_z_position < adj_z_front_low)
        {
          move_z(10, 1400);
        }
        else if (current_z_position > adj_z_front_low)
        {
          move_z(-10, 1400);
        }
      }
    }
  }
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}
void small_right_curve()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  long start_point_x = adj_x_center;
  long start_point_y = adj_y_center + half_distance;
  move_y(half_distance, 1400);
  delay(500);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  long pre_x_internal_point = adj_x_center;
  long pre_y_internal_point = adj_y_center + half_distance;
  double step = PI / 1000;

  for (double angle = 0; angle < PI; angle = angle + step)
  {
    double x_internal_point_1 = adj_x_center - half_distance * sin(angle);
    double y_internal_point_1 = adj_y_center + half_distance * cos(angle);
    double x_move_distance = x_internal_point_1 - pre_x_internal_point;
    double y_move_distance = y_internal_point_1 - pre_y_internal_point;
    myStepper_x.move(long(x_move_distance));
    myStepper_y.move(long(y_move_distance));
    float x_speed;
    float y_speed;
    x_speed = sqrt(pow(1400, 2) / (1 + pow((y_move_distance / x_move_distance), 2)));
    y_speed = sqrt(pow(1400, 2) / (1 + pow((x_move_distance / y_move_distance), 2)));
    while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
    {
      myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + long(x_move_distance), myStepper_x.currentPosition(), x_speed));
      myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + long(y_move_distance), myStepper_y.currentPosition(), y_speed));
      if (myStepper_x.distanceToGo() != 0)
      {
        myStepper_x.run();
      }
      if (myStepper_y.distanceToGo() != 0)
      {
        myStepper_y.run();
      }
    }
    pre_x_internal_point = x_internal_point_1;
    pre_y_internal_point = y_internal_point_1;
  }
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}
void s_curve()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  long start_point_x = adj_x_center;
  long start_point_y = adj_y_center + full_distance;
  move_y(full_distance, 1400);
  delay(500);
  int z_moveDown_distance = adj_z_front_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  long pre_x_internal_point = start_point_x;
  long pre_y_internal_point = start_point_y;
  double step = PI / 1000;

  for (double angle = 0; angle < PI; angle = angle + step)
  {
    double x_internal_point_1 = adj_x_center - (half_distance * sin(angle));
    double y_internal_point_1 = adj_y_center + (half_distance + (half_distance * cos(angle)));
    double x_move_distance = x_internal_point_1 - pre_x_internal_point;
    double y_move_distance = y_internal_point_1 - pre_y_internal_point;
    myStepper_x.move(long(x_move_distance));
    myStepper_y.move(long(y_move_distance));
    float x_speed;
    float y_speed;
    x_speed = sqrt(pow(1400, 2) / (1 + pow((y_move_distance / x_move_distance), 2)));
    y_speed = sqrt(pow(1400, 2) / (1 + pow((x_move_distance / y_move_distance), 2)));
    while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
    {
      myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + long(x_move_distance), myStepper_x.currentPosition(), x_speed));
      myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + long(y_move_distance), myStepper_y.currentPosition(), y_speed));
      if (myStepper_x.distanceToGo() != 0)
      {
        myStepper_x.run();
      }
      if (myStepper_y.distanceToGo() != 0)
      {
        myStepper_y.run();
      }
    }
    pre_x_internal_point = x_internal_point_1;
    pre_y_internal_point = y_internal_point_1;
    int current_z_position = myStepper_z.currentPosition();
    if (abs(current_z_position - adj_z_center_low) > 30)
    {
      if (current_z_position < adj_z_center_low)
      {
        move_z(10, 1400);
      }
      else if (current_z_position > adj_z_center_low)
      {
        move_z(-10, 1400);
      }
    }
  }

  for (double angle = 0; angle < PI; angle = angle + step)
  {
    double x_internal_point_1 = adj_x_center + half_distance * cos(angle);
    double y_internal_point_1 = adj_y_center -(half_distance - half_distance * cos(angle));
    double x_move_distance = x_internal_point_1 - pre_x_internal_point;
    double y_move_distance = y_internal_point_1 - pre_y_internal_point;
    myStepper_x.move(long(x_move_distance));
    myStepper_y.move(long(y_move_distance));
    float x_speed;
    float y_speed;
    x_speed = sqrt(pow(1400, 2) / (1 + pow((y_move_distance / x_move_distance), 2)));
    y_speed = sqrt(pow(1400, 2) / (1 + pow((x_move_distance / y_move_distance), 2)));
    while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0)
    {
      myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() + long(x_move_distance), myStepper_x.currentPosition(), x_speed));
      myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() + long(y_move_distance), myStepper_y.currentPosition(), y_speed));
      if (myStepper_x.distanceToGo() != 0)
      {
        myStepper_x.run();
      }
      if (myStepper_y.distanceToGo() != 0)
      {
        myStepper_y.run();
      }
    }
    pre_x_internal_point = x_internal_point_1;
    pre_y_internal_point = y_internal_point_1;
    int current_z_position = myStepper_z.currentPosition();
    if (abs(current_z_position - adj_z_back_low) > 30)
    {
      if (current_z_position < adj_z_back_low)
      {
        move_z(10, 1400);
      }
      else if (current_z_position > adj_z_back_low)
      {
        move_z(-10, 1400);
      }
    }
  }
}
void h_left()
{
  Serial.println("Now execute horizonal left");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(-full_distance_at_45, full_distance_at_45, 1400);
  int z_moveDown_distance = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  delay(100);
  int z_adjustment = ((adj_z_front_low + adj_z_left_low) / 2) - ((adj_z_front_low + adj_z_right_low) / 2);
  move_x_y_z_in_task(7071, 0, z_adjustment);
  delay(100);
  move_z(100, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Horizonal left executed");
}
void h_right()
{
  Serial.println("Now execute horizonal right");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(full_distance_at_45, -full_distance_at_45, 1400);
  int z_moveDown_distance = ((adj_z_back_low + adj_z_left_low) / 2) - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  delay(100);
  int z_adjustment = ((adj_z_back_low + adj_z_left_low) / 2) - ((adj_z_back_low + adj_z_right_low) / 2);
  move_x_y_z_in_task(-7071, 0, z_adjustment);
  delay(100);
  move_z(100, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Horizonal left executed");
}
void v_forward()
{
  Serial.println("Now execute virtical forward");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(-half_distance_at_45, -half_distance_at_45, 1400);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  delay(100);
  move_y(5000, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Virtical forward executed");
}
void v_backward()
{
  Serial.println("Now execute virtical backward");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(half_distance_at_45, half_distance_at_45, 1400);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  delay(100);
  move_y(-5000, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Virtical backward executed");
}
void multi_fb()
{
  Serial.println("Now execute virtical forward and backward");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = adj_z_front_low - adj_z_center_low;
  move_x_y_z_in_task(0, full_distance, z_adjustment);
  delay(100);
  move_x_y_z_in_task(0, -full_distance, -z_adjustment);
  delay(100);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Executed virtical forward and backward");
}
void multi_incomplete_t()
{
  Serial.println("Now execute mutiple direction 1");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(-full_distance_at_45, full_distance_at_45, z_adjustment);
  delay(100);
  int z_adjustment_2 = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_right_low;
  myStepper_x.move(-long(1465));
  myStepper_y.move(-full_distance_at_45);
  myStepper_z.move(z_adjustment_2);
  float x_speed;
  float y_speed;
  float z_speed;
  float time_tmp = full_distance_at_45 / 1400;
  x_speed = sqrt(pow(1400, 2) / (1 + pow((full_distance_at_45 / 1465), 2)));
  y_speed = sqrt(pow(1400, 2) / (1 + pow((1465 / full_distance_at_45), 2)));
  z_speed = abs(z_adjustment_2 / time_tmp);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0 || myStepper_z.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() - long(1758), myStepper_x.currentPosition(), x_speed));
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() - long(4242), myStepper_y.currentPosition(), y_speed));
    myStepper_z.setSpeed(cal_speed_dir(myStepper_z.currentPosition() + z_adjustment_2, myStepper_z.currentPosition(), z_speed));
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
  delay(100);
  move_z(100, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Executed mutiple direction 1");
}
void multi_complete_t()
{
  Serial.println("Now execute mutiple direction 2");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  delay(500);
  move_z(z_moveDown_distance, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center_low;
  move_x_y_z_in_task(-full_distance_at_45, full_distance_at_45, z_adjustment);
  delay(100);
  int z_adjustment_2 = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_right_low;
  myStepper_x.move(-long(1465));
  myStepper_y.move(-full_distance_at_45);
  myStepper_z.move(z_adjustment_2);
  float x_speed;
  float y_speed;
  float z_speed;
  float time_tmp = full_distance_at_45 / 1400;
  x_speed = sqrt(pow(1400, 2) / (1 + pow((full_distance_at_45 / 1465), 2)));
  y_speed = sqrt(pow(1400, 2) / (1 + pow((1465 / full_distance_at_45), 2)));
  z_speed = abs(z_adjustment_2 / time_tmp);
  while (myStepper_x.distanceToGo() != 0 || myStepper_y.distanceToGo() != 0 || myStepper_z.distanceToGo() != 0)
  {
    myStepper_x.setSpeed(cal_speed_dir(myStepper_x.currentPosition() - long(1758), myStepper_x.currentPosition(), x_speed));
    myStepper_y.setSpeed(cal_speed_dir(myStepper_y.currentPosition() - long(4242), myStepper_y.currentPosition(), y_speed));
    myStepper_z.setSpeed(cal_speed_dir(myStepper_z.currentPosition() + z_adjustment_2, myStepper_z.currentPosition(), z_speed));
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
  delay(100);
  int z_adjustment_3 = -adj_z_right_low + adj_z_center_low;
  move_x_y_z_in_task(full_distance, 0, z_adjustment);
  delay(100);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Executed mutiple direction 2");
}
//===========================      ENDs      =====================//

void vib_touch_center()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  int z_moveDown_distance = adj_z_center_low - adj_z_center;
  move_z(z_moveDown_distance, 1400);
  digitalWrite(vib_motor_1, HIGH);
  delay(300);
  digitalWrite(vib_motor_1, LOW);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}

void vib_run_1s()
{
  digitalWrite(vib_motor_1, HIGH);
  delay(300); // change to 0.3 sec.
  digitalWrite(vib_motor_1, LOW);
}
//=====================  Vibration tactile patterns  =============//
void point_16_vib()
{
  Serial.println("Now execute point 16 vib.");
  vib_touch_center();
  move_y(full_distance, 1400);
  int z_adjustment = adj_z_front_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 16 vib Excuted. standby.");
}
void point_8_vib()
{
  Serial.println("Now execute point 8.");
  vib_touch_center();
  move_y(half_distance, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 8 Excuted. standby.");
}
void point_1_vib()
{
  Serial.println("Now execute point 1.");
  vib_touch_center();
  move_x_y_parallel(-full_distance_at_45, full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 1 Excuted. standby.");
}
void point_9_vib()
{
  Serial.println("Now execute point 9.");
  vib_touch_center();
  move_x_y_parallel(-half_distance_at_45, half_distance_at_45, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 9 Excuted. standby.");
}
void point_2_vib()
{
  Serial.println("Now execute point 10.");
  vib_touch_center();
  move_x(-full_distance, 1400);
  int z_adjustment = adj_z_right_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 2 Excuted. standby.");
}
void point_10_vib()
{
  Serial.println("Now execute point 10 vib.");
  vib_touch_center();
  move_x(-half_distance, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 10 vib Excuted. standby.");
}
void point_3_vib()
{
  Serial.println("Now execute point 3 vib.");
  vib_touch_center();
  move_x_y_parallel(-full_distance_at_45, -full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_back_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 3 vib Excuted. standby.");
}
void point_11_vib()
{
  Serial.println("Now execute point 11 vib.");
  vib_touch_center();
  move_x_y_parallel(-half_distance_at_45, -half_distance_at_45, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 11 vib Excuted. standby.");
}
void point_4_vib()
{
  Serial.println("Now execute point 4 vib.");
  vib_touch_center();
  move_y(-full_distance, 1400);
  int z_adjustment = adj_z_back_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 4 vib Excuted. standby.");
}
void point_12_vib()
{
  Serial.println("Now execute point 12 vib.");
  vib_touch_center();
  move_y(-half_distance, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 12 vib Excuted. standby.");
}
void point_5_vib()
{
  Serial.println("Now execute point 5 vib.");
  vib_touch_center();
  move_x_y_parallel(full_distance_at_45, -full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_back_low + adj_z_left_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 5 vib Excuted. standby.");
}
void point_13_vib()
{
  Serial.println("Now execute point 13 vib.");
  vib_touch_center();
  move_x_y_parallel(half_distance_at_45, -half_distance_at_45, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 13 vib Excuted. standby.");
}
void point_6_vib()
{
  Serial.println("Now execute point 6 vib.");
  vib_touch_center();
  move_x(full_distance, 1400);
  int z_adjustment = adj_z_left_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 6 vib Excuted. standby.");
}
void point_14_vib()
{
  Serial.println("Now execute point 14 vib.");
  vib_touch_center();
  move_x(half_distance, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 14 vib Excuted. standby.");
}
void point_7_vib()
{
  Serial.println("Now execute point 7 vib.");
  vib_touch_center();
  move_x_y_parallel(full_distance_at_45, full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_left_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 7 vib Excuted. standby.");
}
void point_15_vib()
{
  Serial.println("Now execute point 15 vib.");
  vib_touch_center();
  move_x_y_parallel(half_distance_at_45, half_distance_at_45, 700);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("point 15 vib Excuted. standby.");
}
void big_left_curve_vib()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_y(-full_distance, 1400);
  int z_adjustment = adj_z_back_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  long temp = full_distance / sqrt(2);
  move_x_y_parallel(temp, long(full_distance - temp), 1400);
  z_adjustment = (adj_z_back_low + adj_z_left_low) / 2 - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(long(full_distance - temp), temp, 1400);
  z_adjustment = adj_z_left_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-long(full_distance - temp), temp, 1400);
  z_adjustment = (adj_z_front_low + adj_z_left_low) / 2 - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-temp, long(full_distance - temp), 1400);
  z_adjustment = (adj_z_front_low + adj_z_left_low) / 2 - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}
void small_right_curve_vib()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_y(half_distance, 1400);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  long temp = half_distance / sqrt(2);
  move_x_y_parallel(-temp, -long(half_distance - temp), 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-long(half_distance - temp), -temp, 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(long(half_distance - temp), -temp, 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(temp, -long(half_distance - temp), 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);

  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}
void s_curve_vib()
{
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_y(full_distance, 1400);
  int z_adjustment = adj_z_front_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-half_distance_at_45, -(full_distance - half_distance_at_45), 1400);
  z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  vib_touch_center();

  move_x_y_parallel(half_distance_at_45, -half_distance_at_45, 1400);
  z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-half_distance_at_45, -(full_distance - half_distance_at_45), 1400);
  z_adjustment = adj_z_back_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);

  move_to_center(adj_x_center, adj_y_center, adj_z_center);
}
void h_left_vib()
{
  Serial.println("Now execute horizonal left");

  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(-full_distance_at_45, full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);
  move_x(7071, 1400);
  int z_adjustment_2 = ((adj_z_front_low + adj_z_left_low) / 2) - adj_z_center;
  move_z(z_adjustment_2, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Horizonal left executed");
}
void h_right_vib()
{
  Serial.println("Now execute horizonal left");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(full_distance_at_45, -full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_back_low + adj_z_back_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);
  move_x(-7071, 1400);
  int z_adjustment_2 = ((adj_z_back_low + adj_z_back_low) / 2) - adj_z_center;
  move_z(z_adjustment_2, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Horizonal left executed");
}
void v_forward_vib()
{
  Serial.println("Now execute virtical forward");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(-half_distance_at_45, -half_distance_at_45, 1400);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);
  move_y(5000, 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Virtical forward executed");
}
void v_backward_vib()
{
  Serial.println("Now execute virtical backward");
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  move_x_y_parallel(half_distance_at_45, half_distance_at_45, 1400);
  int z_adjustment = adj_z_center_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_z(-z_adjustment, 1400);
  move_y(-5000, 1400);
  move_z(z_adjustment, 1400);
  vib_run_1s();
  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Virtical backward executed");
}
void multi_fb_vib()
{
  Serial.println("Now execute virtical forward and backward");
  point_16_vib();
  vib_touch_center();
  Serial.println("Executed virtical forward and backward");
}
void multi_incomplete_t_vib()
{
  Serial.println("Now execute mutiple direction 1");
  vib_touch_center();
  move_x_y_parallel(-full_distance_at_45, full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-(full_distance-full_distance_at_45), -full_distance_at_45, 1400);
  z_adjustment = adj_z_right_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_to_center(adj_x_center, adj_y_center, adj_z_center);
  Serial.println("Executed mutiple direction 1");
}
void multi_complete_t_vib()
{
  Serial.println("Now execute mutiple direction 2");
  vib_touch_center();
  move_x_y_parallel(-full_distance_at_45, full_distance_at_45, 1400);
  int z_adjustment = ((adj_z_front_low + adj_z_right_low) / 2) - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  move_x_y_parallel(-(full_distance-full_distance_at_45), -full_distance_at_45, 1400);
  z_adjustment = adj_z_right_low - adj_z_center;
  move_z(z_adjustment, 1400);
  vib_run_1s();

  move_z(-z_adjustment, 1400);
  vib_touch_center();
  Serial.println("Executed mutiple direction 2");
}
//===========================      ENDs      =====================//