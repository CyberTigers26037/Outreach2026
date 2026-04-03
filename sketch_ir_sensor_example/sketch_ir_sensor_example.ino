#define leftSensor  A0
#define rightSensor A1

void setup() {
  pinMode(leftSensor,INPUT);
  pinMode(rightSensor,INPUT);

  Serial.begin(9600);
}

void loop(){
  if (digitalRead(leftSensor) && digitalRead(rightSensor)) {
    Serial.println ("Black under both sensors. Stopping");
  }
  else if (digitalRead(leftSensor)) {
    Serial.println ("Black under left sensor. Turning left.");
  }
  else if (digitalRead(rightSensor)) {
    Serial.println ("Black under right sensor. Turning right");
  }
}