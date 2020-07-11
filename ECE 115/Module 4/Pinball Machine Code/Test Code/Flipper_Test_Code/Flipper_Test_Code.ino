const int solenoidLeft = 26;//flipper left blue wire
const int solenoidRight = 25;//flipper right blue wire

//button pins
const int buttonRight = 30;//fires right flipper white jumper wire
const int buttonLeft = 31;//fires left flipper green wire

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(solenoidLeft, OUTPUT);
  pinMode(solenoidRight, OUTPUT);

  //button pin setting
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);
  

}

void loop() {
  // put your main code here, to run repeatedly:
  int Right = digitalRead(buttonRight);
  int Left = digitalRead(buttonLeft);
  Serial.println(Right);
  
  if (Right == HIGH)
  {
    digitalWrite(solenoidRight, HIGH);
    //delay(20);
  }
  else 
  {
    digitalWrite(solenoidRight, LOW);
  }

  if(Left == HIGH)
  {
    digitalWrite(solenoidLeft, HIGH);
    //delay(20);
  }
  else 
  {
    digitalWrite(solenoidLeft, LOW);
  }

}
