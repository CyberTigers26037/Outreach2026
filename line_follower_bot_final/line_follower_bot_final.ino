#include <SparkFun_TB6612.h>

#define AIN1 2
#define AIN2 4
#define PWMA 3
#define STBY 9
#define SPEED -100
#define BIN1 6
#define BIN2 7
#define PWMB 5
#define leftSensor  A0
#define rightSensor A1

const int offsetA = -1;
const int offsetB = -1;

Motor leftMotor = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, offsetB, STBY);

void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop(){
  if (digitalRead(leftSensor) && digitalRead(rightSensor)) {
    Serial.println("Stopping");
    brake(leftMotor, rightMotor);
  }
  else if (digitalRead(leftSensor)) {
    Serial.println("Turning Left");
    left(leftMotor, rightMotor, SPEED);
  }
  else if (digitalRead(rightSensor)) {
    Serial.println("Turning Right");
    right(leftMotor, rightMotor, SPEED);
  }
  else if (!digitalRead(leftSensor) && !digitalRead(rightSensor)) {
    Serial.println("Driving Forward");
    forward(leftMotor, rightMotor, SPEED);
  }
}