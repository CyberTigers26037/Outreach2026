#define leftSensor  A0
#define rightSensor A1

void setup() {
  pinMode(leftSensor,INPUT);
  pinMode(rightSensor,INPUT);

  Serial.begin(9600);
}

void loop(){
  Serial.print("L: ");
  Serial.print(digitalRead(leftSensor));
  Serial.print(", R: ");
  Serial.println(digitalRead(rightSensor));
}