import processing.serial.*;

Serial myPort;
String val;
float background_color ;


int rectX, rectY;      // Position of square button
int circleX, circleY;  // Position of circle button
int rectSize = 90;
int rectCornerR = 12;
int circleSize = 93;   // Diameter of circle
color rectColor, circleColor, baseColor;
color rectHighlight, circleHighlight;
color currentColor;
boolean rectOver = false;
boolean circleOver = false;


void setup() {
  size (1200, 1500);
  background(51);
  xyz_movement_section();
  log_section();
  point_selection();
  pattern_selection();
  myPort = new Serial (this, "COM3", 115200);
}


void draw() {
  update(mouseX, mouseY);
  // Expand array size to the number of bytes you expect
  byte[] inBuffer = new byte[60];
  if (myPort.available() > 0) {
    myPort.readBytesUntil(10, inBuffer);
    String myString = new String(inBuffer);
    println(myString);
  }
}


void xyz_movement_section() {
  fill(255, 255, 255);
  textSize(40);
  text("Move X-Y-Z", 40, 60);
  textSize(50);
  text("X", 40, 200);
  text("Y", 440, 200);
  text("Z", 840, 200);
  rect(100, 135, 100, 100, 8);
  rect(240, 135, 100, 100, 8);
  rect(500, 135, 100, 100, 8);
  rect(640, 135, 100, 100, 8);
  rect(900, 135, 100, 100, 8);
  rect(1040, 135, 100, 100, 8);
  drawArrow(180, 185, 60, 180, 5);
  drawArrow(260, 185, 60, 0, 5);
  drawArrow(550, 215, 60, 270, 5);
  drawArrow(690, 155, 60, 90, 5);
  fill(51);
  text("UP", 920, 200);
  text("DW", 1060, 200);
  fill(255, 255, 255);
  strokeWeight(0);
  rect(0, 300, 1200, 10);
}


void log_section() {
  fill(255, 255, 255);
  textSize(40);
  text("Log adjusted probe location", 40, 370);
  fill(255, 255, 255);
  textSize(40);
  rect(150, 500, 240, 100, 8);
  circle(270, 450, 75);
  circle(270, 650, 75);
  circle(100, 550, 75);
  circle(440, 550, 75);
  rect(600, 500, 150, 100, 8);
  rect(775, 500, 150, 100, 8);
  rect(950, 500, 150, 100, 8);
  rect(775, 375, 150, 100, 8);
  rect(775, 625, 150, 100, 8);
  fill(0);
  textSize(32);
  text("Left Z", 630, 560);
  text("Center Z", 790, 560);
  text("Right Z", 975, 560);
  text("Front Z", 800, 435);
  text("Back Z", 805, 685);
  text("Log center point", 163, 560);
  text("F", 261, 459);
  text("B", 261, 662);
  text("L", 94, 558);
  text("R", 432, 558);
  fill(255, 255, 255);
  strokeWeight(0);
  rect(0, 780, 1200, 10);
}

void point_selection() {
  fill(255, 255, 255);
  textSize(40);
  text("Point selection", 40, 850);
  strokeWeight(6);
  stroke(255, 255, 255);
  line(290, 925, 290, 1425);
  line(40, 1175, 540, 1175);
  line(113, 998, 467, 1352);
  line(468, 998, 113, 1352);
  fill(255, 200, 255);
  strokeWeight(0);
  circle(290, 1175, 50);
  circle(290, 925, 50);
  circle(290, 1050, 50);
  circle(290, 1300, 50);
  circle(290, 1425, 50);
  circle(40, 1175, 50);
  circle(165, 1175, 50);
  circle(415, 1175, 50);
  circle(540, 1175, 50);
  circle(113, 998, 50);
  circle(202, 1087, 50);
  circle(372, 1263, 50);
  circle(467, 1352, 50);
  circle(467, 998, 50);
  circle(372, 1087, 50);
  circle(202, 1263, 50);
  circle(113, 1352, 50);
  fill(255, 255, 255);
  rect(595, 780, 10, 920);
}


void pattern_selection() {
  fill(255, 255, 255);
  textSize(40);
  text("Movement pattern selection", 640, 850);
  rect(650, 880, 200, 100, 8);
  rect(910, 880, 200, 100, 8);
    rect(650, 1000, 200, 100, 8);
  rect(910, 1000, 200, 100, 8);
    rect(650, 1120, 200, 100, 8);
  rect(910, 1120, 200, 100, 8);
    rect(650, 1240, 200, 100, 8);
  rect(910, 1240, 200, 100, 8);
    rect(650, 1360, 200, 100, 8);
  rect(910, 1360, 200, 100, 8);
  
  
  
  
}




void drawArrow(int cx, int cy, int len, float angle, int w) {
  strokeWeight(w);
  pushMatrix();
  translate(cx, cy);
  rotate(radians(angle));
  line(0, 0, len, 0);
  line(len, 0, len - 8, -8);
  line(len, 0, len - 8, 8);
  popMatrix();
}

void update(int x, int y) {
  if ( overCircle(circleX, circleY, circleSize) ) {
    circleOver = true;
    rectOver = false;
  } else if ( overRect(rectX, rectY, rectSize, rectSize) ) {
    rectOver = true;
    circleOver = false;
  } else {
    circleOver = rectOver = false;
  }
}

void mousePressed() {
  if (circleOver) {
    currentColor = circleColor;
  }
  if (rectOver) {
    currentColor = rectColor;
  }
}

boolean overRect(int x, int y, int width, int height) {
  if (mouseX >= x && mouseX <= x+width &&
    mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}

boolean overCircle(int x, int y, int diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}
