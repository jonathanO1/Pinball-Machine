int buzzerPin = A1;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  tone(buzzerPin, 900);
  delay(1000);
  noTone(buzzerPin);
  delay(1000);

}
