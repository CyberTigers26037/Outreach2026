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
  minFwdBack = readValueOrDefault(maxFwdBackAddr, maxFwdBack);
  minFwdBack = readValueOrDefault(minRotateAddr, minRotate);
  minFwdBack = readValueOrDefault(maxRotateAddr, maxRotate);
  minFwdBack = readValueOrDefault(minUpDownAddr, minUpDown);
  minFwdBack = readValueOrDefault(maxUpDownAddr, maxUpDown);
  minFwdBack = readValueOrDefault(minClawAddr, minClaw);
  minFwdBack = readValueOrDefault(maxClawAddr, maxClaw);

}

void loop() {
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

  fwdBackServoPos = adjustServo(leftY, fwdBackServo, fwdBackServoPos, minFwdBack, maxFwdBack);

  Serial.print("Fwd/Back: ");
  Serial.print(fwdBackServoPos);

  upDownServoPos = adjustServo(rightY, upDownServo, upDownServoPos, minUpDown, maxUpDown);

  Serial.print(", up/Down: ");
  Serial.print(upDownServoPos);


  rotateServoPos = adjustServo(leftX, rotateServo, rotateServoPos, minRotate, maxRotate);

  Serial.print(", rotate: ");
  Serial.print(rotateServoPos);

  clawServoPos = adjustServo(rightX, clawServo, clawServoPos, minClaw, maxClaw);

  Serial.print(", claw: ");
  Serial.println(clawServoPos);

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

float adjustServo(int input, Servo servo, float pos, float min, float max) {
  float val = analogRead(input);         
  val = map(val, 0, 1023, -5, 5);

  // create a dead zone around 0 where the joystick doesn't work
  if (abs(val) <= 1) {
    val = 0;
  }

  pos += val / 5;
  
  // constrain the servo to the min/max values
  pos = constrain(pos, min, max);
  servo.write(pos);  
  return pos;
}

