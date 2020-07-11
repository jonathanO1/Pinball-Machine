const int testSolenoid = 26;
const int buttonLeft = 31;
const int buttonRight = 30;
const int solenoidRight = 25;

int Right;
int Left;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(testSolenoid, OUTPUT);

  pinMode(buttonLeft, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  Left = digitalRead(buttonLeft);
  digitalWrite(testSolenoid, Left);
  Serial.println(Left);

  Right = digitalRead(buttonRight);
  digitalWrite(solenoidRight, Right);
  
  
  

/*
  digitalWrite(testSolenoid, HIGH);
  delay(1000);
  digitalWrite(testSolenoid, LOW);
  delay(1000);
  */
  
  

}
