#include <Servo.h>
const int servo = 13;

Servo Servo1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Servo1.attach(servo);

}

void loop() {
  // put your main code here, to run repeatedly:
  Servo1.write(180);
  delay(1000);
  Servo1.write(0);
  delay(1000)w;

}
