#include <SparkFun_TB6612.h>

#define AIN1 2
#define AIN2 4
#define PWMA 3
#define STBY 9
#define SPEED 100

const int offsetA = -1;

Motor leftMotor = Motor(AIN1, AIN2, PWMA, offsetA, STBY);

void setup() {
  Serial.begin(9600);
}

void loop(){
  Serial.println("Driving Forward");
  leftMotor.drive(SPEED);
  delay(3000);
  Serial.println("Driving Backward");
  leftMotor.drive(-SPEED);
  delay(3000);
  Serial.println("Stopping");
  leftMotor.brake();
  delay(3000);
}