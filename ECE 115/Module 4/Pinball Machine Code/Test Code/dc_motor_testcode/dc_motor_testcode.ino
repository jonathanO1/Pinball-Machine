const int motor = 2;
void setup() {
  // put your setup code here, to run once:
  pinMode(motor, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  analogWrite(motor, 150);

}
