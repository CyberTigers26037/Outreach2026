#include <Servo.h>
#include <EEPROM.h>

#define LED_BLINK_DURATION 500 // 500ms on, 500ms off

// declare all of our servos
Servo fwdBackServo;
Servo upDownServo;
Servo clawServo;
Servo rotateServo;  

// declare all of our joysticks
int leftX = A2;
int leftY = A1;
int leftButton = A0;
int rightX = A5;
int rightY = A4;
int rightButton = A3;

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

// Calibration variables...
int lastCalibrationPosition = 0;
bool leftButtonDown = false;
bool rightButtonDown = false;
bool leftButtonPressed = false;
bool rightButtonPressed = false;
bool leftButtonReleased = false;
bool rightButtonReleased = false;
bool oldLeftButtonDown = false;
bool oldRightButtonDown = false;
long buttonTimer = 0;
bool inCalibrationModeLeftJoystick = false;
bool inCalibrationModeRightJoystick = false;
int blinksRemaining = 0;
long ledBlinkTimer = 0;
bool blinkLedIsOn = false;

enum CalibrationStep {
  START,
  FWD_BACK_1,
  UP_DOWN_1,
  FWD_BACK_2,
  UP_DOWN_2,
  ROTATE_1,
  CLAW_1,
  ROTATE_2,
  CLAW_2,
  SAVE,
  STOP
};
CalibrationStep calibrationStep = START;

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

  Serial.print("MIN Fwd/Back: ");
  Serial.println(minFwdBack);
  Serial.print("MAX Fwd/Back: ");
  Serial.println(maxFwdBack);
  Serial.print("MIN up/Down: ");
  Serial.println(minRotate);
  Serial.print("MAX up/Down: ");
  Serial.println(maxRotate);
  Serial.print("MIN rotate: ");
  Serial.println(minUpDown);
  Serial.print("MAX rotate: ");
  Serial.println(maxUpDown);
  Serial.print("MIN claw: ");
  Serial.println(minClaw);
  Serial.print("MAX claw: ");
  Serial.println(maxClaw);

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(rightButton, INPUT_PULLUP);
  pinMode(leftButton, INPUT_PULLUP);

  fwdBackServo.write(fwdBackServoPos);
  rotateServo.write(rotateServoPos);
  upDownServo.write(upDownServoPos);
  clawServo.write(clawServoPos);

  blinkLed(3);
}

void loop() {
  readCalibrationButtonStates();

  if (inCalibrationModeLeftJoystick) {
    runCalibrationStepsLeftJoystick();
  }
  else if (inCalibrationModeRightJoystick) {
    runCalibrationStepsRightJoystick();
  }
  else {
    // Normal operation mode. Respect min/max values...
    rotateServoPos  = adjustServo(leftX, rotateServo, rotateServoPos, minRotate, maxRotate, "Rotate: ");
    fwdBackServoPos = adjustServo(leftY, fwdBackServo, fwdBackServoPos, minFwdBack, maxFwdBack, "Fwd/Back: ");
    clawServoPos    = adjustServo(rightX, clawServo, clawServoPos, minClaw, maxClaw, "Claw: ");
    upDownServoPos  = adjustServo(rightY, upDownServo, upDownServoPos, minUpDown, maxUpDown, "Up/Down: ");

    enterCalibrationModeIfNeeded();
  }

  blinkLedIfNeeded();

  // wait for servos to move
  delay(15);
}

void readCalibrationButtonStates() {
  oldLeftButtonDown = leftButtonDown;
  oldRightButtonDown = rightButtonDown;

  leftButtonDown = (digitalRead(leftButton) == LOW);
  rightButtonDown = (digitalRead(rightButton) == LOW);
  leftButtonPressed = leftButtonDown && !oldLeftButtonDown;
  rightButtonPressed = rightButtonDown && !oldRightButtonDown;
  leftButtonReleased = oldLeftButtonDown && !leftButtonDown;
  rightButtonReleased = oldRightButtonDown && !rightButtonDown;
}

void runCalibrationStepsLeftJoystick() {
  switch (calibrationStep) {
    case START:
      calibrationStep = FWD_BACK_1;
      break;
    case FWD_BACK_1:
      fwdBackServoPos = adjustServo(leftY, fwdBackServo, fwdBackServoPos, minFwdBack, maxFwdBack, "Fwd/Back: ");
      if (leftButtonPressed) {
        lastCalibrationPosition = fwdBackServoPos;
        calibrationStep = FWD_BACK_2;
      }
      break;
    case FWD_BACK_2:
      fwdBackServoPos = adjustServo(leftY, fwdBackServo, fwdBackServoPos, minFwdBack, maxFwdBack, "Fwd/Back: ");
      if (leftButtonPressed) {
        minFwdBack = (lastCalibrationPosition > fwdBackServoPos) ? fwdBackServoPos : lastCalibrationPosition;
        maxFwdBack = (lastCalibrationPosition > fwdBackServoPos) ? lastCalibrationPosition : fwdBackServoPos;
        Serial.print("Saving minFwdBack as ");
        Serial.println(minFwdBack);
        Serial.print("Saving maxFwdBack as ");
        Serial.println(maxFwdBack);
        blinkLed(2);
        calibrationStep = ROTATE_1;
      }
      break;
    case ROTATE_1:
      rotateServoPos  = adjustServo(leftX, rotateServo, rotateServoPos, minRotate, maxRotate, "Rotate: ");
      if (leftButtonPressed) {
        lastCalibrationPosition = rotateServoPos;
        calibrationStep = ROTATE_2;
      }
      break;
    case ROTATE_2:
      rotateServoPos  = adjustServo(leftX, rotateServo, rotateServoPos, minRotate, maxRotate, "Rotate: ");
      if (leftButtonPressed) {
        minRotate = (lastCalibrationPosition > rotateServoPos) ? rotateServoPos : lastCalibrationPosition;
        maxRotate = (lastCalibrationPosition > rotateServoPos) ? lastCalibrationPosition : rotateServoPos;
        Serial.print("Saving minRotate as ");
        Serial.println(minRotate);
        Serial.print("Saving maxRotate as ");
        Serial.println(maxRotate);
        calibrationStep = SAVE;
      }
      break;
    case SAVE:
      EEPROM.write(minFwdBackAddr, minFwdBack);
      EEPROM.write(maxFwdBackAddr, maxFwdBack);
      EEPROM.write(minRotateAddr, minRotate);
      EEPROM.write(maxRotateAddr, maxRotate);
      calibrationStep = STOP;
      break;
    case STOP:
      blinkLed(3);
      Serial.println("Exiting calibration mode for left joystick");
      inCalibrationModeLeftJoystick = false;
      break;
  }
}

void runCalibrationStepsRightJoystick() {
  switch (calibrationStep) {
    case START:
      calibrationStep = UP_DOWN_1;
      break;
    case UP_DOWN_1:
      upDownServoPos = adjustServo(rightY, upDownServo, upDownServoPos, minUpDown, maxUpDown, "Up/Down: ");
      if (rightButtonPressed) {
        lastCalibrationPosition = upDownServoPos;
        calibrationStep = UP_DOWN_2;
      }
      break;
    case UP_DOWN_2:
      upDownServoPos = adjustServo(rightY, upDownServo, upDownServoPos, minUpDown, maxUpDown, "Up/Down: ");
      if (rightButtonPressed) {
        minUpDown = (lastCalibrationPosition > upDownServoPos) ? upDownServoPos : lastCalibrationPosition;
        maxUpDown = (lastCalibrationPosition > upDownServoPos) ? lastCalibrationPosition : upDownServoPos;
        Serial.print("Saving minUpDown as ");
        Serial.println(minUpDown);
        Serial.print("Saving maxUpDown as ");
        Serial.println(maxUpDown);
        blinkLed(2);
        calibrationStep = CLAW_1;
      }
      break;
    case CLAW_1:
      clawServoPos  = adjustServo(rightX, clawServo, clawServoPos, minClaw, maxClaw, "Claw: ");
      if (rightButtonPressed) {
        lastCalibrationPosition = clawServoPos;
        calibrationStep = CLAW_2;
      }
      break;
    case CLAW_2:
      clawServoPos  = adjustServo(rightX, clawServo, clawServoPos, minClaw, maxClaw, "Claw: ");
      if (rightButtonPressed) {
        minClaw = (lastCalibrationPosition > clawServoPos) ? clawServoPos : lastCalibrationPosition;
        maxClaw = (lastCalibrationPosition > clawServoPos) ? lastCalibrationPosition : clawServoPos;
        Serial.print("Saving minClaw as ");
        Serial.println(minClaw);
        Serial.print("Saving maxClaw as ");
        Serial.println(maxClaw);
        calibrationStep = SAVE;
      }
      break;
    case SAVE:
      EEPROM.write(minUpDownAddr, minUpDown);
      EEPROM.write(maxUpDownAddr, maxUpDown);
      EEPROM.write(minClawAddr, minClaw);
      EEPROM.write(maxClawAddr, maxClaw);
      calibrationStep = STOP;
      break;
    case STOP:
      blinkLed(3);
      Serial.println("Exiting calibration mode for right joystick");
      inCalibrationModeRightJoystick = false;
      break;
  }

}

void enterCalibrationModeIfNeeded() {
  if (leftButtonPressed || rightButtonPressed) {
    startTimer(buttonTimer);
  }
  else if (leftButtonDown || rightButtonDown) {
    if (hasTimeElapsed(buttonTimer, 3000)) {

      if (leftButtonDown) {
        Serial.println("Entering calibration mode for left stick");
        inCalibrationModeLeftJoystick = true;
        minFwdBack = 0;
        maxFwdBack = 180;
        minRotate = 0;
        maxRotate = 180;
        blinkLed(3);
      }
      else {
        Serial.println("Entering calibration mode for right stick");
        inCalibrationModeRightJoystick = true;
        minUpDown = 0;
        maxUpDown = 180;
        minClaw = 0;
        maxClaw = 180;
        blinkLed(3);
      }
      calibrationStep = START;
      stopTimer(buttonTimer);
    }
  }
  else if (leftButtonReleased || rightButtonReleased) {
    stopTimer(buttonTimer);
  }
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

void startTimer(long &timer) {
  timer = millis();
}

void stopTimer(long &timer) {
  timer = 0;
}

bool isTimerStarted(long timer) {
  return (timer != 0);
}

bool hasTimeElapsed(long timer, long milliseconds) {
  if (timer == 0) return false;

  return (millis() > (timer + milliseconds));
}

void blinkLed(int blinkCount) {
  blinksRemaining = blinkCount;
  if (blinkCount > 0) {
    // set the blink timer to expired so it fires immediately
    ledBlinkTimer = 1;
  }
}

void blinkLedIfNeeded() {
  if (blinksRemaining == 0) return;

  if (hasTimeElapsed(ledBlinkTimer, LED_BLINK_DURATION)) {
    if (blinkLedIsOn) {
      digitalWrite(LED_BUILTIN, LOW);
      blinksRemaining--;
      blinkLedIsOn = false;
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      blinkLedIsOn = true;
    }
    startTimer(ledBlinkTimer);
  }
}