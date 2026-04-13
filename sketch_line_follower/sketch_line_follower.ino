
/* 
Library used: Adafruit Motor Shield library V1 version: 1.0.1
For this code to run as expected: 
1.The centre to centre distance between the Line sensors should be 11 to 11.5 cm
2. The width of black tape should be 4.8 to 5 cm
3. The distance of the sensor LED from the flat ground surface should be 2 cm.
*/
#include <SparkFun_TB6612.h>
// MACROS for Debug print, while calibrating set its value to 1 else keep it 0
#define DEBUG_PRINT 0
// MACROS for Analog Input
#define LEFT_IR A0
#define RIGHT_IR A1
// MACROS to control the Robot
#define DETECT_LIMIT 300
#define FORWARD_SPEED 60
#define TURN_SHARP_SPEED 150
#define TURN_SLIGHT_SPEED 120
#define DELAY_AFTER_TURN 140
#define BEFORE_TURN_DELAY 10
// BO Motor control related data here
// Here motors are running using M3 and M4 of the shield and Left Motor is connected to M3 and Right Motor is connected to M4 using IC2 of the shield
#define AIN1 2
#define AIN2 4
#define PWMA 3
#define STBY 9
#define SPEED 100
#define BIN1 6
#define BIN2 7
#define PWMB 5
const int offsetA = -1;
const int offsetB = -1;
// variables to store the analog values
int left_value;
int right_value;
// Set the last direction to Stop
char lastDirection = 'S';  
Motor leftMotor = Motor(AIN1, AIN2, PWMA, offsetA, STBY);
Motor rightMotor = Motor(BIN1, BIN2, PWMB, offsetB, STBY);
void setup() {
#if DEBUG_PRINT  
  Serial.begin(9600);
#endif  
  // Set the current speed of Left Motor to 0

  delay(40);  // delay of 40 ms
}
void loop() {
  left_value = analogRead(LEFT_IR);
  right_value = analogRead(RIGHT_IR);
#if DEBUG_PRINT
  // This is for debugging. To check the analog inputs the DETECT_LIMIT MACRO value 300 is set by analysing the debug prints
  Serial.print(left_value);
  Serial.print(",");
  Serial.print(right_value);
  Serial.print(",");
  Serial.print(lastDirection);
  Serial.write(10);
#endif
  // Right Sensor detects black line and left does not detect
  if (right_value >= DETECT_LIMIT && !(left_value >= DETECT_LIMIT)) {
    turnRight();
  }
  // Left Sensor detects black line and right does not detect
  else if ((left_value >= DETECT_LIMIT) && !(right_value >= DETECT_LIMIT)) {
    turnLeft();
  }
  // both sensors doesn't detect black line
  else if (!(left_value >= DETECT_LIMIT) && !(right_value >= DETECT_LIMIT)) {
    moveForward();
  }
  // both sensors detect black line
  else if ((left_value >= DETECT_LIMIT) && (right_value >= DETECT_LIMIT)) {
    stop();
  }
}
void moveForward() {
  if (lastDirection != 'F') {
    // To provide starting push to Robot when last direction was not forward
    forward(leftMotor,rightMotor,255);
    lastDirection = 'F';
    delay(20);
  } else {
    // If the last direction was forward
    forward(leftMotor,rightMotor,FORWARD_SPEED);
  }
}
void stop() {
  if (lastDirection != 'S') {
    // When stop is detected move further one time to check if its actual stop or not, needed when the robot turns
    forward(leftMotor,rightMotor,255);
    lastDirection = 'S';
    delay(40);
  } else {
    // When stop is detected next time then stop the Robot
    brake(leftMotor,rightMotor);
    lastDirection = 'S';
  }
}
void turnRight(void) {
  // If first time Right Turn is taken
  if (lastDirection != 'R') {
    lastDirection = 'R';
    // Stop the motor for some time
    brake(leftMotor,rightMotor);
    delay(BEFORE_TURN_DELAY);
    // take Slight Right turn
    right(leftMotor,rightMotor,TURN_SLIGHT_SPEED);
  } else {
    // take sharp Right turn
    right(leftMotor,rightMotor,TURN_SHARP_SPEED);
  }
  delay(DELAY_AFTER_TURN);
}
void turnLeft() {
  // If first time Left Turn is taken
  if (lastDirection != 'L') {
    lastDirection = 'L';
    // Stop the motor for some time
    brake(leftMotor,rightMotor);
    delay(BEFORE_TURN_DELAY);
    // take slight Left turn
    left(leftMotor,rightMotor,TURN_SLIGHT_SPEED);
  } else {
    // take sharp Left turn
    left(leftMotor,rightMotor,TURN_SHARP_SPEED);
  }
  delay(DELAY_AFTER_TURN);
}