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

float minFwdBack = 0;
float maxFwdBack = 180;

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
  Serial.println(fwdBackServoPos);
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
