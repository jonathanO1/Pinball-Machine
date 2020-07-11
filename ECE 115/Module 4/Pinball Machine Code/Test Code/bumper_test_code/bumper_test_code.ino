const int bumper1 = 3;
const int solenoid1 = 4;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(bumper1, INPUT);
  pinMode(solenoid1, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  int test = digitalRead(bumper1);
  if (test == HIGH)
  {
    digitalWrite(solenoid1, HIGH);
    delay(20);
    digitalWrite(solenoid1, LOW);
    delay(20);
    
  }
  else{
    digitalWrite(solenoid1, LOW);
  }
  Serial.println(test);
  delay(1);

}
