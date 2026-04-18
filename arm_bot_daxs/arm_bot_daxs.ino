#include <Servo.h>

Servo topServo;
Servo xServo;
Servo yServo;

void setup() {
  topServo.attach(9);
  xServo.attach(8);
  yServo.attach(7);
}

void loop() {
 xServo.write(0);
 delay(1000);
 yServo.write(0);
 delay(1000);
 xServo.write(20);
 delay(1000);
 yServo.write(20);
 delay(1000);
}