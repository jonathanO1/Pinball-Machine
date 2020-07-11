/*
 * Flipper and bumper and score test code integration
 * 3/7
 * Currently have everything integrated and fully working pinball machine that satisfies all requirements
 */

//including the servo library
#include <Servo.h>
//Hardware Pins (all subject to changing, be sure to check when setting system up)
//ignore the notes telling color of wire
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

// Global pin values

const int dataPin = 4;  // blue wire to 74HC595 pin 14, serial pin
const int latchPin = 5; // green to 74HC595 pin 12, RCLK
const int clockPin = 6; // yellow to 74HC595 pin 11, SRCLK
const int display1 = 7;//yellow wire for first seven segment display
const int display2 = 8;//blue wire for the second seven segment display
const int display3 = 9;//yellow wire for the third seven segment display
const int display4 = 10;//blue wire for the fourth seven segment display

//declaring the servo pin and calling the servo function from the library
const int servo = 13;
Servo Servo1;

const int irLivesSensor = 2;

const int huaweiTarget = A2;

const int buzzerPin = A1;

const int dcMotorPin = 12;

const int bumperPoints = 23;

const int buttonStart = 32;


int points;
int lives;
bool livesFlag;
bool targetFlag;
int livesLEDs[] = {33,34,35};

enum States {START, PLAY, END};
int State; 

int starttime;
int endtime;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // initialize I/O pins

  pinMode(buzzerPin, OUTPUT);
  
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(display1, OUTPUT);
  pinMode(display2, OUTPUT);
  pinMode(display3, OUTPUT);
  pinMode(display4, OUTPUT);
  //setting all the displays off
  digitalWrite(display1, HIGH);
  digitalWrite(display2, HIGH);
  digitalWrite(display3, HIGH);
  digitalWrite(display4, HIGH);

  pinMode(solenoidBlueL, OUTPUT);
  pinMode(solenoidBlackM, OUTPUT);
  pinMode(solenoidBlueR, OUTPUT);
  pinMode(solenoidLeft, OUTPUT);
  pinMode(solenoidRight, OUTPUT);
  
  pinMode(irLivesSensor, INPUT);
  pinMode(huaweiTarget, INPUT);

  //bumper input setting
  pinMode(bumperL, INPUT);
  pinMode(bumperM, INPUT);
  pinMode(bumperR, INPUT);
  
  //button pin setting
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);

  pinMode(livesLEDs[0], OUTPUT);
  pinMode(livesLEDs[1], OUTPUT);
  pinMode(livesLEDs[2], OUTPUT);

  pinMode(dcMotorPin, OUTPUT);

  Servo1.attach(servo);

  //setup variables
  points = 0;
  livesFlag = false;
  targetFlag = false;
  States State = START;

  

}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.println(State);
  switch(State)
  {
    
    case START:
      Servo1.write(0);
      lives = 3;
      if (digitalRead(buttonStart) ==  HIGH)
      {
        State = PLAY;//Starting the play when button is pressed
        lives = 3;//magic number
        points = 0;
        Buzzer();//there to ring for feedback for states
        
      }
      else if(digitalRead(buttonStart) == LOW)
      {
        State = START;
      }
    break;
      
    case PLAY:
      //Serial.println(lives);
      Servo1.write(45);
      Flipper();
      Bumper();
      mainDisplay(points);
      Lives(lives);
      bonusTarget();
      
      dcMotorRun();
      
      //begginning of the state machine control
      if (lives == 0)
      {
        State = END;
      }
      else
      {
        State = PLAY;
      }
      //end of the state machine control
      
      break;
      //end of the play state

    case END:
      Servo1.write(90);
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(livesLEDs[i], LOW);
      }

      //Writing the flippers low to keep solenoids from burning
      digitalWrite(solenoidLeft, LOW);
      digitalWrite(solenoidRight, LOW);
      //turning the motor off
      dcMotorStop();

      starttime = millis();
      endtime = starttime;
      while((endtime - starttime) <= 3000)
      {
        mainDisplay(points);//displays the final score
        endtime = millis();
      }
      
      State = START;
      break;
      //end of the END state

  }

}


/*
 * Flipper function
 * 
 */
void Flipper()
{
  int Right = digitalRead(buttonRight);
  int Left = digitalRead(buttonLeft);

  
  //Serial.println(Right);

  digitalWrite(solenoidRight, Right);
  digitalWrite(solenoidLeft, Left);
  
  
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
    points = points + bumperPoints;
    digitalWrite(solenoidBlueL, HIGH);
    delay(30);
    digitalWrite(solenoidBlueL, LOW);
    delay(20);
    Buzzer();
  }
  else //if (bumperReadL == LOW)
  {
    digitalWrite(solenoidBlueL, LOW);
  }


  //code below in the if statement operating the right bumper
  if(bumperReadM == HIGH) 
  {
    points = points + bumperPoints;
    digitalWrite(solenoidBlackM, HIGH);
    delay(40);
    digitalWrite(solenoidBlackM, LOW);
    delay(20);
    Buzzer();
  }
  else
  {
    digitalWrite(solenoidBlackM, LOW);
  }


  //if statement below operates the right bumper
  if(bumperReadR == HIGH)
  {
    points = points + bumperPoints;
    digitalWrite(solenoidBlueR, HIGH);
    delay(30);
    digitalWrite(solenoidBlueR, LOW);
    delay(20);
    Buzzer();
  }
  else //if (bumperReadR == LOW)
  {
    digitalWrite(solenoidBlueR, LOW);
  }
  
}

/*
 * bonsu target notes
 * reads from the ir sensor to detect if the target is hit
 * 
 */
void bonusTarget()
{
   int hit = analogRead(huaweiTarget);
   if(hit > 300 && targetFlag == false)
   {
      points = points + 200;
      Buzzer();
   }
   else
   {
      targetFlag == true;
   }
}

/*
 * mainDisplay is the mother function that takes int and displays it on display
 * param - displayedNum is the int to be displayed on the seven segment display
 */
void mainDisplay(int displayedNum)
{
  const int arr_size = 4;
  int arr[arr_size];//holds the value so each digit can be processed individually
  //below parses num into individual digits on array
  for (int i = 0; i < arr_size; i++)
  {
    int digit = displayedNum % 10;
    displayedNum /= 10;//reducing the sie of the num
    arr[arr_size-1-i] = digit;//put the number into the array
  }
  //below displays the numbers one by one on the display
  for(int i = 0; i < arr_size; i++)
  {
    byte num = myfnNumToBits(arr[i]);//separates the num to array
    int numdisplay = i + display1; //sets the num display to be turned on
    //Serial.println(arr[i]);//tests which display is on
    myfnUpdateDisplay(num, numdisplay);//turns on display to display num
    
  }
}
 
  


/*
 * Function takes the bits writes to shift register for the 
 * seven-segment display to display
 * Param eightbits-parameter of bits sent to be displayed
 * param displayNum-specifies which display to turn on
 */
void myfnUpdateDisplay(byte eightBits, int displayNum) 
{
  digitalWrite(displayNum, LOW);//turning display on
  
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
  
  delay(5);//delay for each seperate display
  
  digitalWrite(displayNum, HIGH); //turning display off before turning on next display
  
}


/*
 * function that turns the int value and converts to bits
 * param-someNumber is the int that will be converted
 */
byte myfnNumToBits(int someNumber) {
  switch (someNumber) {
    case 0:
      return B00011000;//done
      break;
    case 1:
      return B11011101;//done
      break;
    case 2:
      return B00101001;//done
      break;
    case 3:
      return B10001001;//done
      break;
    case 4:
      return B11001100;//done
      break;
    case 5:
      return B10001010;//done
      break;
    case 6:
      return B00001010;//done
      break;
    case 7:
      return B11011001;//done
      break;
    case 8:
      return B00001000;//done
      break;
    case 9:
      return B10001000;//done
      break;
    
  }
}

/*
 * Buzzer
 * simply beeps the buzzer for differnt situations
 */
void Buzzer()
{
  tone(buzzerPin, 900);
  delay(200);
  noTone(buzzerPin);
}

/*
 * dc motor functions beloew to run and stop the dc
 * motor when running the pinball machine
 * 
 */
void dcMotorRun()
{
  analogWrite(dcMotorPin, 200);
}

void dcMotorStop()
{
  analogWrite(dcMotorPin, 0);
}



/*
 * Keeps track of the number of lives and drives
 * the LEDs to feedback to the user
 * 
 */
void Lives(int livesRemaining)
{
  int readLives = digitalRead(irLivesSensor);
  
  //turning the lights on based on the number of lives remaining
  for(int i = 0; i < livesRemaining; i++)
  {
    //Serial.println(i);
    digitalWrite(livesLEDs[i], HIGH);
  }

  //turning the lights off when loss of life
  for (int a = livesRemaining; a < 3; a++)
  {
    //Serial.println(livesRemaining);
    digitalWrite(livesLEDs[a], LOW);
  }
  
  //reading for everytime the ball crosses the ir lives sensor at the bottom
  if (readLives == HIGH && livesFlag == false)
  {
    Buzzer();
    livesFlag == true;
    lives = lives - 1;
  }else if(readLives == LOW)
  {
    livesFlag == false;
  }
}
