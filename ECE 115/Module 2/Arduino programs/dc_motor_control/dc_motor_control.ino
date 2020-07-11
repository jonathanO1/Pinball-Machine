/*
Adafruit Arduino - Lesson 13. DC Motor
*/
 
 
int motorPin = 3;
int potPin = A0;
 
void setup() 
{ 
  pinMode(motorPin, OUTPUT);
  pinMode(potPin, INPUT);
  Serial.begin(9600);
  //while (! Serial);
  //Serial.println("Speed 0 to 255");
} 
 
 
void loop() 
{ 
  int val = analogRead(potPin);
  //Serial.println(val);
  int sp = map(val,0, 857, 0, 255);//mapping from dc 
  Serial.println(sp);
  analogWrite(motorPin, 10);
  delay(400);
} 
