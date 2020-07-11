int photoresistor = A2;
const int photDig = 2;


void setup() {
  Serial.begin(9600);
  pinMode(photoresistor, INPUT);
  pinMode(photDig, INPUT);
  //pinMode(irLight, OUTPUT);

}

void loop() {
  int val = analogRead(photoresistor);
  int digRead = digitalRead(photDig);
  Serial.println(digRead);
  //Serial.println(val);
  
  delay(10);
}
