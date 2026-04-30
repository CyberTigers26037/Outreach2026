#include <Servo.h>
#include <EEPROM.h>

// declare all of our servos
Servo fwdBackServo;
Servo upDownServo;
Servo clawServo;
Servo rotateServo;  

// declare all of our joysticks
int leftY = A0;
int leftX = A1;
int rightY = A2;
int rightX = A3;
int rightButton = A4;
int leftButton = A5;

float fwdBackServoPos = 90;
float rotateServoPos = 90;
float upDownServoPos = 90;
float clawServoPos = 90;

int minFwdBackAddr = 0;
int maxFwdBackAddr = 1;
int minRotateAddr = 2;
int maxRotateAddr = 3;
int minUpDownAddr = 4;
int maxUpDownAddr = 5;
int minClawAddr = 6;
int maxClawAddr = 7;

int minFwdBack = 45;
int maxFwdBack = 143;
int minRotate = 56;
int maxRotate = 123;
int minUpDown = 68;
int maxUpDown = 118;
int minClaw = 90;
int maxClaw = 131;

void setup() {
  fwdBackServo.attach(3);
  upDownServo.attach(9);
  clawServo.attach(6);
  rotateServo.attach(5);
  Serial.begin(9600);

  minFwdBack = readValueOrDefault(minFwdBackAddr, minFwdBack);
  maxFwdBack = readValueOrDefault(maxFwdBackAddr, maxFwdBack);
  minRotate = readValueOrDefault(minRotateAddr, minRotate);
  maxRotate = readValueOrDefault(maxRotateAddr, maxRotate);
  minUpDown = readValueOrDefault(minUpDownAddr, minUpDown);
  maxUpDown = readValueOrDefault(maxUpDownAddr, maxUpDown);
  minClaw = readValueOrDefault(minClawAddr, minClaw);
  maxClaw = readValueOrDefault(maxClawAddr, maxClaw);

  pinMode(rightButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);

  fwdBackServo.write(fwdBackServoPos);
  rotateServo.write(rotateServoPos);
  upDownServo.write(upDownServoPos);
  clawServo.write(clawServoPos);
}

void loop() {
  /*
  Serial.print("MIN Fwd/Back: ");
  Serial.print(minFwdBack);
  Serial.print("MAX Fwd/Back: ");
  Serial.print(maxFwdBack);
  Serial.print(", MIN up/Down: ");
  Serial.print(minRotate);
  Serial.print(", MAX up/Down: ");
  Serial.print(maxRotate);
  Serial.print(", MIN rotate: ");
  Serial.print(minUpDown);
  Serial.print(",MAX rotate: ");
  Serial.print(maxUpDown);
  Serial.print(", MIN claw: ");
  Serial.print(minClaw);
  Serial.print(", MAX claw: ");
  Serial.print(maxClaw);
  */
  fwdBackServoPos = adjustServo(leftY, fwdBackServo, fwdBackServoPos, minFwdBack, maxFwdBack, "Fwd/Back: ");
  upDownServoPos = adjustServo(rightY, upDownServo, upDownServoPos, minUpDown, maxUpDown, "Up/Down: ");
  rotateServoPos = adjustServo(leftX, rotateServo, rotateServoPos, minRotate, maxRotate, "Rotate: ");
  clawServoPos = adjustServo(rightX, clawServo, clawServoPos, minClaw, maxClaw, "Claw: ");

  //int btn = digitalRead(leftButton);
  if (digitalRead(leftButton) == LOW) {
    Serial.println("left button pressed");
  }

  //btn = digitalRead(rightButton);
  if (digitalRead(rightButton) == LOW) {
    Serial.println("right button pressed");
  }

  // wait for servos to move
  delay(15);
}

int readValueOrDefault(int addr, int defaultValue) {
  int temp = EEPROM.read(addr);
  if ((temp >= 0) && (temp <= 180)) {
    return temp;
  }
  return defaultValue;
}

float adjustServo(int input, Servo servo, float pos, float min, float max, char title[]) {
  float joystickInput = analogRead(input);         
  joystickInput = map(joystickInput, 0, 1023, -5, 5);

  // create a dead zone around 0 where the joystick doesn't work
  if (abs(joystickInput) <= 1) {
    joystickInput = 0;
  }

  pos += (joystickInput / 5);
  
  // constrain the servo to the min/max values
  pos = round(constrain(pos, min, max));

  if (pos != servo.read()) {
    Serial.print(title);
    Serial.println((int)pos);
    servo.write(pos);
  }
  return pos;
}

