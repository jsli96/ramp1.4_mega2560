import processing.serial.*;
import controlP5.*;

ControlP5 cp5;

Serial myPort;
String val;
float background_color ;
public int move_distance = 0;
// ====================================
// Solution for MAC is 600 * 900
// Full scale is 1200 * 1800
int screen_x = 600;
int screen_y = 900;
// ====================================
float ratio = screen_x / 1200.0;
boolean xLeft = false;
boolean xRight = false;
boolean yLeft = false;
boolean yRight = false;
boolean zLeft = false;
boolean zRight = false;
boolean logCenterPoint = false;
boolean l = false;
boolean r = false;
boolean f = false;
boolean b = false;
boolean logCenterZ = false;
boolean logLeftZ = false;
boolean logRightZ = false;
boolean logFrontZ = false;
boolean logBackZ = false;
boolean static_center = false;
boolean static_1 = false;
boolean static_2 = false;
boolean static_3 = false;
boolean static_4 = false;
boolean static_5 = false;
boolean static_6 = false;
boolean static_7 = false;
boolean static_8 = false;
boolean static_9 = false;
boolean static_10 = false;
boolean static_11 = false;
boolean static_12 = false;
boolean static_13 = false;
boolean static_14 = false;
boolean static_15 = false;
boolean static_16 = false;
boolean dynamic_1 = false;
boolean dynamic_2 = false;
boolean dynamic_3 = false;
boolean dynamic_4 = false;
boolean dynamic_5 = false;
boolean dynamic_6 = false;
boolean dynamic_7 = false;
boolean dynamic_8 = false;
boolean dynamic_9 = false;
boolean dynamic_10 = false;
boolean motor_start = false;
boolean motor_stop = false;
boolean home = false;
boolean show_current_location = false;
boolean skin_or_vib_button = false;
// ====================================
IntList static_point_list = new IntList();
IntList pattern_list = new IntList();
int m_index = 0;
int n_index = 0;
int skin_vib_choice = 0;


void settings() {
  size (screen_x, screen_y);
}


void setup() {
  background(51);
  xyz_movement_section();
  log_section();
  point_selection();
  pattern_selection();
  one_click();
  myPort = new Serial (this, "/dev/tty.usbserial-2110", 115200);
}


void draw() {
  update_status(mouseX, mouseY);
  byte[] inBuffer = new byte[60];
  if (myPort.available() > 0) {
    myPort.readBytesUntil(10, inBuffer);
    String myString = new String(inBuffer);
    println(myString);
  }
}


void xyz_movement_section() {
  noStroke();
  cp5 = new ControlP5(this);
  cp5.addNumberbox("numberbox")
    .setColorBackground(color(255))
    .setColorValueLabel(color(0))
    .setColorLabel(255)
    .setPosition(250*ratio, 20*ratio)
    .setSize(int(200*ratio), int(60*ratio))
    .setScrollSensitivity(1.1)
    .setValue(200);

  fill(255, 255, 255);
  textSize(40*ratio);
  text("Move X-Y-Z", 40*ratio, 60*ratio);
  rect(550*ratio, 10*ratio, 120*ratio, 80*ratio, 8*ratio);
  rect(700*ratio, 10*ratio, 120*ratio, 80*ratio, 8*ratio);
  rect(850*ratio, 10*ratio, 120*ratio, 80*ratio, 8*ratio);
  rect(1000*ratio, 10*ratio, 120*ratio, 80*ratio, 8*ratio);
  textSize(30*ratio);
  fill(0);
  text("Home", 570*ratio, 60*ratio);
  text("Location", 705*ratio, 60*ratio);
  text("Start", 880*ratio, 60*ratio);
  text("Stop", 1030*ratio, 60*ratio);
  fill(255, 255, 255);
  textSize(50*ratio);
  text("X", 40*ratio, 200*ratio);
  text("Y", 440*ratio, 200*ratio);
  text("Z", 840*ratio, 200*ratio);
  rect(100*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  rect(240*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  rect(500*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  rect(640*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  rect(900*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  rect(1040*ratio, 135*ratio, 100*ratio, 100*ratio, 8*ratio);
  stroke(6);
  drawArrow(180*ratio, 185*ratio, 60*ratio, 180, 5*ratio);
  drawArrow(260*ratio, 185*ratio, 60*ratio, 0, 5*ratio);
  drawArrow(550*ratio, 215*ratio, 60*ratio, 270, 5*ratio);
  drawArrow(690*ratio, 155*ratio, 60*ratio, 90, 5*ratio);
  fill(51);
  text("UP", 920*ratio, 200*ratio);
  text("DW", 1060*ratio, 200*ratio);
  fill(255, 255, 255);
  strokeWeight(0*ratio);
  rect(0*ratio, 300*ratio, 1200*ratio, 10*ratio);
}


void log_section() {
  fill(255, 255, 255);
  textSize(40*ratio);
  text("Log adjusted probe location", 40*ratio, 370*ratio);
  fill(255, 255, 255);
  textSize(40*ratio);
  rect(150*ratio, 500*ratio, 240*ratio, 100*ratio, 8*ratio);
  circle(270*ratio, 450*ratio, 75*ratio);
  circle(270*ratio, 650*ratio, 75*ratio);
  circle(100*ratio, 550*ratio, 75*ratio);
  circle(440*ratio, 550*ratio, 75*ratio);
  rect(600*ratio, 500*ratio, 150*ratio, 100*ratio, 8*ratio);
  rect(775*ratio, 500*ratio, 150*ratio, 100*ratio, 8*ratio);
  rect(950*ratio, 500*ratio, 150*ratio, 100*ratio, 8*ratio);
  rect(775*ratio, 375*ratio, 150*ratio, 100*ratio, 8*ratio);
  rect(775*ratio, 625*ratio, 150*ratio, 100*ratio, 8*ratio);
  fill(0);
  textSize(32*ratio);
  text("Left Z", 630*ratio, 560*ratio);
  text("Center Z", 790*ratio, 560*ratio);
  text("Right Z", 975*ratio, 560*ratio);
  text("Front Z", 800*ratio, 435*ratio);
  text("Back Z", 805*ratio, 685*ratio);
  text("Log center point", 163*ratio, 560*ratio);
  text("F", 261*ratio, 459*ratio);
  text("B", 261*ratio, 662*ratio);
  text("L", 94*ratio, 558*ratio);
  text("R", 432*ratio, 558*ratio);
  fill(255, 255, 255);
  strokeWeight(0);
  rect(0*ratio, 780*ratio, 1200*ratio, 10*ratio);
}

void point_selection() {
  textSize(40*ratio);
  fill(255, 255, 0);
  rect(400*ratio, 810*ratio, 150*ratio, 66*ratio, 6*ratio);
  fill(0);
  text("Skin", 420*ratio, 850*ratio);
  fill(255, 255, 255);
  text("Point selection", 40*ratio, 850*ratio);
  strokeWeight(6*ratio);
  stroke(255, 255, 255);
  line(290*ratio, 925*ratio, 290*ratio, 1425*ratio);
  line(40*ratio, 1175*ratio, 540*ratio, 1175*ratio);
  line(113*ratio, 998*ratio, 467*ratio, 1352*ratio);
  line(468*ratio, 998*ratio, 113*ratio, 1352*ratio);
  fill(255, 200, 255);
  strokeWeight(0*ratio);
  circle(290*ratio, 1175*ratio, 50*ratio);
  circle(290*ratio, 925*ratio, 50*ratio);
  circle(290*ratio, 1050*ratio, 50*ratio);
  circle(290*ratio, 1300*ratio, 50*ratio);
  circle(290*ratio, 1425*ratio, 50*ratio);
  circle(40*ratio, 1175*ratio, 50*ratio);
  circle(165*ratio, 1175*ratio, 50*ratio);
  circle(415*ratio, 1175*ratio, 50*ratio);
  circle(540*ratio, 1175*ratio, 50*ratio);
  circle(113*ratio, 998*ratio, 50*ratio);
  circle(202*ratio, 1087*ratio, 50*ratio);
  circle(372*ratio, 1263*ratio, 50*ratio);
  circle(467*ratio, 1352*ratio, 50*ratio);
  circle(467*ratio, 998*ratio, 50*ratio);
  circle(372*ratio, 1087*ratio, 50*ratio);
  circle(202*ratio, 1263*ratio, 50*ratio);
  circle(113*ratio, 1352*ratio, 50*ratio);
  fill(255, 255, 255);
  rect(595*ratio, 780*ratio, 10*ratio, 700*ratio);
  rect(0*ratio, 1480*ratio, 1200*ratio, 10*ratio);
}


void pattern_selection() {
  fill(255, 255, 255);
  textSize(40*ratio);
  text("Movement pattern selection", 640*ratio, 850*ratio);
  rect(650*ratio, 880*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(910*ratio, 880*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(650*ratio, 1000*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(910*ratio, 1000*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(650*ratio, 1120*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(910*ratio, 1120*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(650*ratio, 1240*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(910*ratio, 1240*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(650*ratio, 1360*ratio, 200*ratio, 100*ratio, 8*ratio);
  rect(910*ratio, 1360*ratio, 200*ratio, 100*ratio, 8*ratio);
  fill(0);
  textSize(32*ratio);
  text("1", 740*ratio, 935*ratio);
  text("3", 740*ratio, 1055*ratio);
  text("5", 740*ratio, 1175*ratio);
  text("7", 740*ratio, 1295*ratio);
  text("9", 740*ratio, 1415*ratio);
  text("2", 1000*ratio, 935*ratio);
  text("4", 1000*ratio, 1055*ratio);
  text("6", 1000*ratio, 1175*ratio);
  text("8", 1000*ratio, 1295*ratio);
  text("10", 990*ratio, 1415*ratio);
}



void one_click() {
  noStroke();
  int myColor = color(255);
  int textColor = color(51);
  cp5 = new ControlP5(this);
  fill(255);
  // create a new button with name 'buttonA'
  cp5.addButton("GenerateList1")
    .setColorBackground(myColor)
    .setColorLabel(textColor)
    .setColorActive(color(127, 255, 212))
    .setColorForeground(color(175))
    .setPosition(40*ratio, 1500*ratio)
    .setSize(int(200*ratio), int(50*ratio))
    .addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_BROADCAST) {
        generate_random_list_1();
      }
    }
  }
  )
  ;

  // and add another 2 buttons
  cp5.addButton("Next_Point")
    .setColorBackground(myColor)
    .setColorLabel(textColor)
    .setColorActive(color(127, 255, 212))
    .setColorForeground(color(175))
    .setValue(0)
    .setPosition(340*ratio, 1500*ratio)
    .setSize(int(200*ratio), int(50*ratio))
    .addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_BROADCAST) {
        serial_write_next_point();
      }
    }
  }
  )
  ;

  cp5.addButton("GenerateList2")
    .setColorBackground(myColor)
    .setColorLabel(textColor)
    .setColorActive(color(127, 255, 212))
    .setColorForeground(color(175))
    .setValue(0)
    .setPosition(640*ratio, 1500*ratio)
    .setSize(int(200*ratio), int(50*ratio))
    .addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_BROADCAST) {
        generate_random_list_2();
      }
    }
  }
  )
  ;

  cp5.addButton("Next_Pattern")
    .setColorBackground(myColor)
    .setColorLabel(textColor)
    .setColorActive(color(127, 255, 212))
    .setColorForeground(color(175))
    .setValue(0)
    .setPosition(940*ratio, 1500*ratio)
    .setSize(int(200*ratio), int(50*ratio))
    .addCallback(new CallbackListener() {
    public void controlEvent(CallbackEvent theEvent) {
      if (theEvent.getAction()==ControlP5.ACTION_BROADCAST) {
        serial_write_next_pattern();
      }
    }
  }
  )
  ;
}



void generate_random_list_1() {
  static_point_list = new IntList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16);
  static_point_list.shuffle();
  println(static_point_list);
}

void generate_random_list_2() {
  pattern_list = new IntList(1, 2, 3, 4, 5, 6, 7, 8, 9, 10);
  pattern_list.shuffle();
  println(pattern_list);
}

void serial_write_next_point() {
  if (skin_vib_choice == 0) {
    if (static_point_list.size() != 0) {
      if (m_index > 15) {
        m_index = 0;
      }
      int tmp= static_point_list.get(m_index);
      println(tmp);
      myPort.write(str(tmp));
      m_index = m_index + 1;
    } else {
      println("Please generate the list first!!!");
    }
  } else if (skin_vib_choice == 1) {
        if (static_point_list.size() != 0) {
      if (m_index > 15) {
        m_index = 0;
      }
      int tmp= static_point_list.get(m_index) + 30;
      println(tmp);
      myPort.write(str(tmp));
      m_index = m_index + 1;
    } else {
      println("Please generate the list first!!!");
    }
    
  }
}

void serial_write_next_pattern() {
  if (pattern_list.size() != 0) {
    if (n_index > 9) {
      n_index = 0;
    }
    println(pattern_list.get(n_index));
    n_index = n_index + 1;
  } else {
    println("Please generate the list first!!!");
  }
}

void drawArrow(float cx, float cy, float len, float angle, float w) {
  strokeWeight(w);
  pushMatrix();
  translate(cx, cy);
  rotate(radians(angle));
  line(0, 0, len, 0);
  line(len, 0, len - 8, -8);
  line(len, 0, len - 8, 8);
  popMatrix();
}


void update_status(int x, int y) {
  xLeft = test_rec(100*ratio, 135*ratio, 100*ratio, 100*ratio);
  xRight = test_rec(240*ratio, 135*ratio, 100*ratio, 100*ratio);
  yLeft = test_rec(500*ratio, 135*ratio, 100*ratio, 100*ratio);
  yRight = test_rec(640*ratio, 135*ratio, 100*ratio, 100*ratio);
  zLeft = test_rec(900*ratio, 135*ratio, 100*ratio, 100*ratio);
  zRight = test_rec(1040*ratio, 135*ratio, 100*ratio, 100*ratio);
  logCenterPoint = test_rec(150*ratio, 500*ratio, 240*ratio, 100*ratio);
  l = overCircle(100*ratio, 550*ratio, 75*ratio);
  r = overCircle(440*ratio, 550*ratio, 75*ratio);
  f = overCircle(270*ratio, 450*ratio, 75*ratio);
  b = overCircle(270*ratio, 650*ratio, 75*ratio);
  logCenterZ = test_rec(775*ratio, 500*ratio, 150*ratio, 100*ratio);
  logLeftZ = test_rec(600*ratio, 500*ratio, 150*ratio, 100*ratio);
  logRightZ = test_rec(950*ratio, 500*ratio, 150*ratio, 100*ratio);
  logFrontZ = test_rec(775*ratio, 375*ratio, 150*ratio, 100*ratio);
  logBackZ = test_rec(775*ratio, 625*ratio, 150*ratio, 100*ratio);
  static_center = overCircle(290*ratio, 1175*ratio, 50*ratio);
  static_1 = overCircle(467*ratio, 998*ratio, 50*ratio);
  static_2 = overCircle(540*ratio, 1175*ratio, 50*ratio);
  static_3 = overCircle(467*ratio, 1352*ratio, 50*ratio);
  static_4 = overCircle(290*ratio, 1425*ratio, 50*ratio);
  static_5 = overCircle(113*ratio, 1352*ratio, 50*ratio);
  static_6 = overCircle(40*ratio, 1175*ratio, 50*ratio);
  static_7 = overCircle(113*ratio, 998*ratio, 50*ratio);
  static_8 = overCircle(290*ratio, 1050*ratio, 50*ratio);
  static_9 = overCircle(372*ratio, 1087*ratio, 50*ratio);
  static_10 = overCircle(415*ratio, 1175*ratio, 50*ratio);
  static_11 = overCircle(372*ratio, 1263*ratio, 50*ratio);
  static_12 = overCircle(290*ratio, 1300*ratio, 50*ratio);
  static_13 = overCircle(202*ratio, 1263*ratio, 50*ratio);
  static_14 = overCircle(165*ratio, 1175*ratio, 50*ratio);
  static_15 = overCircle(202*ratio, 1087*ratio, 50*ratio);
  static_16 = overCircle(290*ratio, 925*ratio, 50*ratio);
  dynamic_1 = test_rec(650*ratio, 880*ratio, 200*ratio, 100*ratio);
  dynamic_2 = test_rec(910*ratio, 880*ratio, 200*ratio, 100*ratio);
  dynamic_3 = test_rec(650*ratio, 1000*ratio, 200*ratio, 100*ratio);
  dynamic_4 = test_rec(910*ratio, 1000*ratio, 200*ratio, 100*ratio);
  dynamic_5 = test_rec(650*ratio, 1120*ratio, 200*ratio, 100*ratio);
  dynamic_6 = test_rec(910*ratio, 1120*ratio, 200*ratio, 100*ratio);
  dynamic_7 = test_rec(650*ratio, 1240*ratio, 200*ratio, 100*ratio);
  dynamic_8 = test_rec(910*ratio, 1240*ratio, 200*ratio, 100*ratio);
  dynamic_9 = test_rec(650*ratio, 1360*ratio, 200*ratio, 100*ratio);
  dynamic_10 = test_rec(910*ratio, 1360*ratio, 200*ratio, 100*ratio);
  motor_start = test_rec(850*ratio, 10*ratio, 120*ratio, 80*ratio);
  motor_stop = test_rec(1000*ratio, 10*ratio, 120*ratio, 80*ratio);
  home = test_rec(550*ratio, 10*ratio, 120*ratio, 80*ratio);
  show_current_location = test_rec(700*ratio, 10*ratio, 120*ratio, 80*ratio);
  skin_or_vib_button = test_rec(400*ratio, 810*ratio, 150*ratio, 66*ratio);
}

boolean test_rec(float x, float y, float width, float height) {
  if (mouseX >= x && mouseX <= x+width &&
    mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}

boolean overCircle(float x, float y, float diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}

void numberbox(int value) {
  move_distance = value;
}

void mousePressed() {
  if (skin_or_vib_button) {
    if (skin_vib_choice == 0) {
      println("Change to vib");
      fill(255, 255, 0);
      rect(400*ratio, 810*ratio, 150*ratio, 66*ratio, 6*ratio);
      fill(0);
      text("Vib", 420*ratio, 850*ratio);
      skin_vib_choice = 1;
      m_index = 0;
    } else {
      println("Change to skin");
      fill(255, 255, 0);
      rect(400*ratio, 810*ratio, 150*ratio, 66*ratio, 6*ratio);
      fill(0);
      text("Skin", 420*ratio, 850*ratio);
      skin_vib_choice = 0;
      m_index = 0;
    }
  }

  if (xLeft) {
    println("xLeft");
    myPort.write("x," + move_distance);
  } else if (xRight) {
    println("xRight");
    myPort.write("x,-" + move_distance);
  } else if (yLeft) {
    println("yFront");
    myPort.write("y," + move_distance);
  } else if (yRight) {
    println("yBack");
    myPort.write("y,-" + move_distance);
  } else if (zLeft) {
    println("zUp");
    myPort.write("z," + move_distance);
  } else if (zRight) {
    println("zDown");
    myPort.write("z,-" + move_distance);
  } else if (logCenterPoint) {
    println("logCenterPoint");
    myPort.write("log xyz");
  } else if (l) {
    println("l");
    myPort.write("x,5000");
  } else if (r) {
    println("r");
    myPort.write("x,-5000");
  } else if (f) {
    println("f");
    myPort.write("y,5000");
  } else if (b) {
    println("b");
    myPort.write("y,-5000");
  } else if (logCenterZ) {
    println("logCenterZ");
    myPort.write("log center z lowest point");
  } else if (logLeftZ) {
    println("logLeftZ");
    myPort.write("log left z lowest point");
  } else if (logRightZ) {
    println("logRightZ");
    myPort.write("log right z lowest point");
  } else if (logFrontZ) {
    println("logFrontZ");
    myPort.write("log front z lowest point");
  } else if (logBackZ) {
    println("logBackZ");
    myPort.write("log back z lowest point");
  } else if (static_center) {
    println("static_center");
    myPort.write("adjusted home");
  } else if (static_1 && skin_vib_choice == 0) {
    println("static_1");
    myPort.write("1");
  } else if (static_2 && skin_vib_choice == 0) {
    println("static_2");
    myPort.write("2");
  } else if (static_3 && skin_vib_choice == 0) {
    println("static_3");
    myPort.write("3");
  } else if (static_4 && skin_vib_choice == 0) {
    println("static_4");
    myPort.write("4");
  } else if (static_5 && skin_vib_choice == 0) {
    println("static_5");
    myPort.write("5");
  } else if (static_6 && skin_vib_choice == 0) {
    println("static_6");
    myPort.write("6");
  } else if (static_7 && skin_vib_choice == 0) {
    println("static_7");
    myPort.write("7");
  } else if (static_8 && skin_vib_choice == 0) {
    println("static_8");
    myPort.write("8");
  } else if (static_9 && skin_vib_choice == 0) {
    println("static_9");
    myPort.write("9");
  } else if (static_10 && skin_vib_choice == 0) {
    println("static_10");
    myPort.write("10");
  } else if (static_11 && skin_vib_choice == 0) {
    println("static_11");
    myPort.write("11");
  } else if (static_12 && skin_vib_choice == 0) {
    println("static_12");
    myPort.write("12");
  } else if (static_13 && skin_vib_choice == 0) {
    println("static_13");
    myPort.write("13");
  } else if (static_14 && skin_vib_choice == 0) {
    println("static_14");
    myPort.write("14");
  } else if (static_15 && skin_vib_choice == 0) {
    println("static_15");
    myPort.write("15");
  } else if (static_16 && skin_vib_choice == 0) {
    println("static_16");
    myPort.write("16");
  } else if (dynamic_1 && skin_vib_choice == 0) {
    println("dynamic_1");
    myPort.write("21");
  } else if (dynamic_2 && skin_vib_choice == 0) {
    println("dynamic_2");
    myPort.write("22");
  } else if (dynamic_3 && skin_vib_choice == 0) {
    println("dynamic_3");
    myPort.write("23");
  } else if (dynamic_4 && skin_vib_choice == 0) {
    println("dynamic_4");
    myPort.write("24");
  } else if (dynamic_5 && skin_vib_choice == 0) {
    println("dynamic_5");
    myPort.write("25");
  } else if (dynamic_6 && skin_vib_choice == 0) {
    println("dynamic_6");
    myPort.write("26");
  } else if (dynamic_7 && skin_vib_choice == 0) {
    println("dynamic_7");
    myPort.write("27");
  } else if (dynamic_8 && skin_vib_choice == 0) {
    println("dynamic_8");
    myPort.write("28");
  } else if (dynamic_9 && skin_vib_choice == 0) {
    println("dynamic_9");
    myPort.write("29");
  } else if (dynamic_10 && skin_vib_choice == 0) {
    println("dynamic_10");
    myPort.write("30");
  } else if (static_1 && skin_vib_choice == 1) {
    println("static_31_vib");
    myPort.write("31");
  } else if (static_2 && skin_vib_choice == 1) {
    println("static_32");
    myPort.write("32");
  } else if (static_3 && skin_vib_choice == 1) {
    println("static_33");
    myPort.write("33");
  } else if (static_4 && skin_vib_choice == 1) {
    println("static_34");
    myPort.write("34");
  } else if (static_5 && skin_vib_choice == 1) {
    println("static_35");
    myPort.write("35");
  } else if (static_6 && skin_vib_choice == 1) {
    println("static_36");
    myPort.write("36");
  } else if (static_7 && skin_vib_choice == 1) {
    println("static_37");
    myPort.write("37");
  } else if (static_8 && skin_vib_choice == 1) {
    println("static_38");
    myPort.write("38");
  } else if (static_9 && skin_vib_choice == 1) {
    println("static_39");
    myPort.write("39");
  } else if (static_10 && skin_vib_choice == 1) {
    println("static_40");
    myPort.write("40");
  } else if (static_11 && skin_vib_choice == 1) {
    println("static_41");
    myPort.write("41");
  } else if (static_12 && skin_vib_choice == 1) {
    println("static_42");
    myPort.write("42");
  } else if (static_13 && skin_vib_choice == 1) {
    println("static_43");
    myPort.write("43");
  } else if (static_14 && skin_vib_choice == 1) {
    println("static_44");
    myPort.write("44");
  } else if (static_15 && skin_vib_choice == 1) {
    println("static_45");
    myPort.write("45");
  } else if (static_16 && skin_vib_choice == 1) {
    println("static_46");
    myPort.write("46");
  } else if (dynamic_1 && skin_vib_choice == 1) {
    println("dynamic_21");
    myPort.write("51");
  } else if (dynamic_2 && skin_vib_choice == 1) {
    println("dynamic_22");
    myPort.write("52");
  } else if (dynamic_3 && skin_vib_choice == 1) {
    println("dynamic_23");
    myPort.write("53");
  } else if (dynamic_4 && skin_vib_choice == 1) {
    println("dynamic_24");
    myPort.write("54");
  } else if (dynamic_5 && skin_vib_choice == 1) {
    println("dynamic_25");
    myPort.write("55");
  } else if (dynamic_6 && skin_vib_choice == 1) {
    println("dynamic_26");
    myPort.write("56");
  } else if (dynamic_7 && skin_vib_choice == 1) {
    println("dynamic_27");
    myPort.write("57");
  } else if (dynamic_8 && skin_vib_choice == 1) {
    println("dynamic_28");
    myPort.write("58");
  } else if (dynamic_9 && skin_vib_choice == 1) {
    println("dynamic_29");
    myPort.write("59");
  } else if (dynamic_10 && skin_vib_choice == 1) {
    println("dynamic_30");
    myPort.write("60");
  } else if (home) {
    println("home");
    myPort.write("adjusted home");
  } else if (show_current_location) {
    println("Show current location");
    myPort.write("show current location");
  } else if (motor_start) {
    println("Start motors");
    myPort.write("start motors");
  } else if (motor_stop) {
    println("Stop motors");
    myPort.write("stop motors");
  }
}
