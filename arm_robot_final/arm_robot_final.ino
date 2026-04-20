#include <Servo.h>

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

float minFwdBack = 45;
float maxFwdBack = 143;
float minRotate = 56;
float maxRotate = 123;
float minUpDown = 68;
float maxUpDown = 118;
float minClaw = 90;
float maxClaw = 131;

void setup() {
  fwdBackServo.attach(3);
  upDownServo.attach(9);
  clawServo.attach(6);
  rotateServo.attach(5);
  Serial.begin(9600);
}

void loop() {
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
