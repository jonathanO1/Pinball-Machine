//bumper read pins
const int bumperL = 8;//digital input for firing bumper left yellow jumper wire
const int bumperM = 28;//digital input for firing the bumper middle blue wire middle resistor
const int bumperR = 29;//digital input for firing bumper right

//button pins
const int buttonRight = 30;//fires right flipper white jumper wire
const int buttonLeft = 31;//fires left flipper green wire
const int buttonStart = 32;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //bumper input setting
  pinMode(bumperL, INPUT);
  pinMode(bumperM, INPUT);
  pinMode(bumperR, INPUT);
  
  //button pin setting
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);
  pinMode(buttonStart, INPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(bumperR));

}
