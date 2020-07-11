int solenoid1 = 25;//green wire
int solenoid2 = 26;//white wire
//int solenoid3 = 23;//blue wire
int button2 = 30;//yellow wire
int button1 = 31;//green wire

void setup() {
  //setting up the pins for the solenoid
  Serial.begin(9600);
  pinMode(solenoid1, OUTPUT);
  pinMode(solenoid2, OUTPUT);
  //pinMode(solenoid3, OUTPUT);
  pinMode(button2, INPUT);
  pinMode(button1, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  
  int butt = digitalRead(button1);
  int butt2 = digitalRead(button2);
  Serial.println(butt);
  if(butt == HIGH){
    digitalWrite(solenoid1, HIGH);
    //butt = LOW;
  }
  else
  {
    digitalWrite(solenoid1 , LOW);
  }
  
  if(butt2 == HIGH){
    digitalWrite(solenoid2, HIGH);
    butt2 = LOW;
  }else{
    digitalWrite(solenoid2, LOW);
  }
  
  //delay(2);
  /*
  digitalWrite(solenoid3, HIGH);
  delay(500);
  digitalWrite(solenoid3, LOW);
  delay(500);
  */
  

}
