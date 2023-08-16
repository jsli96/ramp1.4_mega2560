import processing.serial.*;

Serial myPort;
String val;
float background_color ;
// ====================================
// The screen size ratio must be 4:5!!!
int screen_x = 600;
int screen_y = 750;
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






void settings() {
  size (screen_x, screen_y);
}


void setup() {
  background(51);
  xyz_movement_section();
  log_section();
  point_selection();
  pattern_selection();
  // myPort = new Serial (this, "COM3", 115200);
}


void draw() {
  update_status(mouseX, mouseY);
  // Expand array size to the number of bytes you expect
  // byte[] inBuffer = new byte[60];
  //if (myPort.available() > 0) {
  //  myPort.readBytesUntil(10, inBuffer);
  //  String myString = new String(inBuffer);
  //  println(myString);
  //}
}


void xyz_movement_section() {
  fill(255, 255, 255);
  textSize(40*ratio);
  text("Move X-Y-Z", 40*ratio, 60*ratio);
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
  fill(255, 255, 255);
  textSize(40*ratio);
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
  rect(595*ratio, 780*ratio, 10*ratio, 920*ratio);
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

void mousePressed() {
  if (xLeft) {
    println("xLeft");
  } else if (xRight) {
    println("xRight");
  } else if (yLeft) {
    println("yFront");
  } else if (yRight) {
    println("yBack");
  } else if (zLeft) {
    println("zUp");
  } else if (zRight) {
    println("zDown");
  } else if (logCenterPoint) {
    println("logCenterPoint");
  } else if (l) {
    println("l");
  } else if (r) {
    println("r");
  } else if (f) {
    println("f");
  } else if (b) {
    println("b");
  } else if (logCenterZ) {
    println("logCenterZ");
  } else if (logLeftZ) {
    println("logLeftZ");
  } else if (logRightZ) {
    println("logRightZ");
  } else if (logFrontZ) {
    println("logFrontZ");
  } else if (logBackZ) {
    println("logBackZ");
  } else if (static_center) {
    println("static_center");
  } else if (static_1) {
    println("static_1");
  } else if (static_2) {
    println("static_2");
  } else if (static_3) {
    println("static_3");
  } else if (static_4) {
    println("static_4");
  } else if (static_5) {
    println("static_5");
  } else if (static_6) {
    println("static_6");
  } else if (static_7) {
    println("static_7");
  } else if (static_8) {
    println("static_8");
  } else if (static_9) {
    println("static_9");
  } else if (static_10) {
    println("static_10");
  } else if (static_11) {
    println("static_11");
  } else if (static_12) {
    println("static_12");
  } else if (static_13) {
    println("static_13");
  } else if (static_14) {
    println("static_14");
  } else if (static_15) {
    println("static_15");
  } else if (static_16) {
    println("static_16");
  } else if (dynamic_1) {
    println("dynamic_1");
  } else if (dynamic_2) {
    println("dynamic_2");
  } else if (dynamic_3) {
    println("dynamic_3");
  } else if (dynamic_4) {
    println("dynamic_4");
  } else if (dynamic_5) {
    println("dynamic_5");
  } else if (dynamic_6) {
    println("dynamic_6");
  } else if (dynamic_7) {
    println("dynamic_7");
  } else if (dynamic_8) {
    println("dynamic_8");
  } else if (dynamic_9) {
    println("dynamic_9");
  } else if (dynamic_10) {
    println("dynamic_10");
  }
}
