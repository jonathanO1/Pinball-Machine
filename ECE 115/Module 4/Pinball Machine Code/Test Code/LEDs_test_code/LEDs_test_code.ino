
int livesLEDs[] = {33,34,35};
int lives;
void setup() {
  // put your setup code here, to run once:
  pinMode(livesLEDs[0], OUTPUT);
  pinMode(livesLEDs[1], OUTPUT);
  pinMode(livesLEDs[2], OUTPUT);

  lives = 3;

}

void loop() {
  // put your main code here, to run repeatedly:
   
  for(int i = 0; i < lives; i++)
  {
    digitalWrite(livesLEDs[i], HIGH);
  }
  for (int a = lives-1; a < lives-1; a++)
  {
    digitalWrite(livesLEDs[a], LOW);
  }
  //digitalWrite(33, HIGH);
  

}
