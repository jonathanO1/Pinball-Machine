seng/*
 * Ver 3
 * All the functionality of Version two but with better code structure, all in functions
 * Todo: Clean up commented code out and comment through everything, 
 * also, test the most effective frequency range for the buzzer
 */

//including the servo library
#include <Servo.h>
//Hardware Pins (all subject to changing, be sure to check when setting system up)
//ignore the notes telling color of wire
//Solenoid pins
const int solenoidBlueR = 22;//bumper left green jumper wire
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

//I don't think the wire descriptions are updated for the display circuit
const int dataPin = 4;  // blue wire to 74HC595 pin 14, serial pin
const int latchPin = 5; // green to 74HC595 pin 12, RCLK
const int clockPin = 6; // yellow to 74HC595 pin 11, SRCLK
const int display1 = 7;//yellow wire for first seven segment display
const int display2 = 8;//blue wire for the second seven segment display
const int display3 = 9;//yellow wire for the third seven segment display
const int display4 = 10;//blue wire for the fourth seven segment display

//declaring the servo pin and calling the servo function from the library
//comment maybe if you have time the wire descriptions for each

const int servo = 13;
Servo Servo1;

const int buttonStart = 32;
const int irLivesSensor = 2;

const int huaweiTarget = A2;

const int buzzerPin = A1;//lol maybe change later to normal pwm pin

const int dcMotorPin = 12;

int livesLEDs[] = {33,34,35};
enum States {START, PLAY, END};
int State; 

//global variables that help run the entire system
int points;
int lives;

bool livesFlag;
bool targetFlag;

bool bumperFlagL;
bool bumperFlagM;
bool bumperFlagR;

const int bumperPoints = 23;
const int bonusPoints = 200;

const int maxLives = 3;

//run the timer function for the endstae
int starttime;
int endtime;



/*
 * Setup Function
 * Sets the pinouts and sets important variables for the game
 */
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

  bumperFlagL = false;
  bumperFlagM = false;
  bumperFlagR = false;

  

}

/*
 * main loop for the code
 * Contains the state machine for the entire system, 
 * 
 */
void loop() {
  // put your main code here, to run repeatedly:
  
  switch(State)
  {
    
    case START:
      Servo1.write(0);
      lives = 3;
      if (digitalRead(buttonStart) ==  HIGH)
      {
        State = PLAY;//Starting the play when button is pressed
        points = 0;
        Buzzer(2000);//there to ring for feedback for states
        
      }
      else if(digitalRead(buttonStart) == LOW)
      {
        ///delay(500);
        State = START;
      }
    break;
    //end of the start state
      
    case PLAY:
      //delay(1000);
      Serial.println(digitalRead(buttonStart));
      Servo1.write(45);
      Flipper();
      Bumper();
      mainDisplay(points);
      Lives(lives);
      bonusTarget();
      dcMotorRun(200);
      
      //beginning of the state machine control
      if (lives == 0)
      {
        State = END;
      }
      //else if(digitalRead(buttonStart == HIGH))//kill switch
      //{
      //  lives = 0;
      //}
      else
      {
        State = PLAY;
      }
      //end of the state machine control

    
      
      break;
      //end of the play state

    case END:
      Servo1.write(90);
      //manually turning all the lights off because function won't do that
      for (int i = 0; i < maxLives; i++)
      {
        digitalWrite(livesLEDs[i], LOW);
      }

      //Writing the flippers low to keep solenoids from burning
      digitalWrite(solenoidLeft, LOW);
      digitalWrite(solenoidRight, LOW);
      //turning the motor off
      dcMotorRun(0);

      //displaying the final score for 3 seconds before going to start state
      starttime = millis();
      endtime = starttime;
      while((endtime - starttime) <= 3000)
      {
        mainDisplay(points);//displays the final score
        endtime = millis();
      }

      //going back to the start state (Loops)
      State = START;
      break;
      //end of the END state

  }

}


/*
 * Flipper function
 * simply runs the flippers on the bumper machine
 * 
 */
void Flipper()
{
  int Right = digitalRead(buttonRight);
  int Left = digitalRead(buttonLeft);
  //simply writing the status of the respective control buttons
  digitalWrite(solenoidRight, Right);
  digitalWrite(solenoidLeft, Left);
  
  
}


/*
 * Bumper function simply reads the bumper tape and fires solenoid and adds points
 * redoing the bumper function
 */
void Bumper()
{
  
  int bumperReadL = digitalRead(bumperL);
  int bumperReadM = digitalRead(bumperM);
  int bumperReadR = digitalRead(bumperR);

  digitalWrite(solenoidBlueL, bumperReadL);
  digitalWrite(solenoidBlackM, bumperReadM);
  digitalWrite(solenoidBlueR, bumperReadR);

  //code for the left bumper points
  if (bumperReadL == HIGH && bumperFlagL == false)
  {
    points = points + bumperPoints;
    bumperFlagL = true;
  }
  else if(bumperReadL == LOW)
  {
    bumperFlagL == false;
  }

  //code for the middle bumper
  if (bumperReadM == HIGH && bumperFlagM == false)
  {
    points = points + bumperPoints;
    bumperFlagM = true;
  }else if (bumperReadM == LOW)
  {
    bumperFlagM = false;
  }

  //code for the right bumper points
  if (bumperReadR == HIGH && bumperFlagR == false)
  {
    points = points + bumperPoints;
    bumperFlagR = true;
  }else if(bumperReadR == LOW)
  {
    bumperFlagR == false;
  }

  
}

/*
 * bonus target notes
 * reads from the ir sensor to detect if the target is hit
 * 
 */
void bonusTarget()
{
   //checking the analog val, 300 arbitary num for an inbetween num
   int hit = analogRead(huaweiTarget);
   if(hit > 300 && targetFlag == false)
   {
      points = points + bonusPoints;
      Buzzer(4500);
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
void Buzzer(int freq)
{
  tone(buzzerPin, freq);
  delay(200);
  noTone(buzzerPin);
}



/*
 * dc motor functions beloew to run and stop the dc
 * motor when running the pinball machine
 * 
 */
void dcMotorRun(int val)
{
  analogWrite(dcMotorPin, val);
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
    digitalWrite(livesLEDs[i], HIGH);
  }

  //turning the lights off when loss of life
  for (int a = livesRemaining; a < 3; a++)
  {
    
    digitalWrite(livesLEDs[a], LOW);
  }
  
  //reading for everytime the ball crosses the ir lives sensor at the bottom
  if (readLives == HIGH && livesFlag == false)
  {
    Buzzer(3000);
    livesFlag == true;
    lives = lives - 1;
  }else if(readLives == LOW)
  {
    livesFlag == false;
  }
}
