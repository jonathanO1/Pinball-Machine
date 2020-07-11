/*
 * Flipper and bumper test code integration
 * 
 * 
 */
//Hardware Pins (all subject to changing, be sure to check when setting system up)
//Solenoid pins
const int solenoidBlueR = 22;//bumper left green humper wire
const int solenoidBlackM = 23;//bumper middle blue wire
const int solenoidBlueL = 24;//bumper right blue wire
const int solenoidLeft = 26;//flipper left blue wire
const int solenoidRight = 25;//flipper right blue wire

//bumper read pins
const int bumperL = 11;//digital input for firing bumper left yellow jumper wire
const int bumperM = 28;//digital input for firing the bumper middle blue wire middle resistor
const int bumperR = 29;//digital input for firing bumper right

//button pins
const int buttonRight = 30;//fires right flipper white jumper wire
const int buttonLeft = 31;//fires left flipper green wire



 
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //solenoid pin setting
  pinMode(solenoidBlueL, OUTPUT);
  pinMode(solenoidBlackM, OUTPUT);
  pinMode(solenoidBlueR, OUTPUT);
  pinMode(solenoidLeft, OUTPUT);
  pinMode(solenoidRight, OUTPUT);

  //bumper input setting
  pinMode(bumperL, INPUT);
  pinMode(bumperM, INPUT);
  pinMode(bumperR, INPUT);
  
  //button pin setting
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
  Flipper();
  Bumper();
    //delay(1);

}




/*
 * Flipper function
 * 
 */
void Flipper()
{
  int Right = digitalRead(buttonRight);
  int Left = digitalRead(buttonLeft);

  
  Serial.println(Right);

  digitalWrite(solenoidRight, Right);
  digitalWrite(solenoidLeft, Left);
  /*
  if (Right == HIGH)
  {
    digitalWrite(solenoidRight, HIGH);
  }
  else 
  {
    digitalWrite(solenoidRight, LOW);
  }

  if(Left == HIGH)
  {
    digitalWrite(solenoidLeft, HIGH);
  }
  else 
  {
    digitalWrite(solenoidLeft, LOW);
  }
  */
  
}


/*
 * Bumper function simply reads the bumper tape and fires solenoid and adds points
 * 
 */
void Bumper()
{
  
  int bumperReadL = digitalRead(bumperL);
  int bumperReadM = digitalRead(bumperM);
  int bumperReadR = digitalRead(bumperR);

  //Serial.println(bumperReadR);

  //code below in the if statement operating the left bumper
  if(bumperReadL == HIGH)
  {
    //points = points + 2;
    digitalWrite(solenoidBlueL, HIGH);
    delay(30);
    digitalWrite(solenoidBlueL, LOW);
    delay(20);
  }
  else //if (bumperReadL == LOW)
  {
    digitalWrite(solenoidBlueL, LOW);
  }


  //code below in the if statement operating the right bumper
  if(bumperReadM == HIGH)
  {
    //points = points + 2;
    digitalWrite(solenoidBlackM, HIGH);
    delay(40);
    digitalWrite(solenoidBlackM, LOW);
    delay(20);
  }
  else// if (bumperReadM == LOW)
  {
    digitalWrite(solenoidBlackM, LOW);
  }


  //if statement below operates the right bumper
  if(bumperReadR == HIGH)
  {
    //points = points + 2;
    digitalWrite(solenoidBlueR, HIGH);
    delay(30);
    digitalWrite(solenoidBlueR, LOW);
    delay(20);
  }
  else //if (bumperReadR == LOW)
  {
    digitalWrite(solenoidBlueR, LOW);
  }
  
}
