/*
 * Pinbal System Code Ver 1
 * currently integrated in flipper code, bumper code, display, and ir and lives
 * 
 * to do, 2/28, install the ir sensors and make sure the code works somewhat
 * add in the lives led's
 * 
 */

//Hardware Pins (all subject to changing, be sure to check when setting system up)
//Solenoid pins
const int solenoidBlueR = 22;//bumper Right
const int solenoidBlackM = 23;//bumper middle blue wire
const int solenoidBlueL = 24;//bumper Left blue wire
const int solenoidLeft = 26;//flipper left blue wire
const int solenoidRight = 25;//flipper right blue wire

//bumper read pins
const int bumperL = 11;//Currently not on 11, remember to move from 8 to 11
const int bumperM = 28;//digital input for firing the bumper middle blue wire middle resistor
const int bumperR = 29;//digital input for firing bumper right

//button pins
const int buttonRight = 30;//fires right flipper white jumper wire
const int buttonLeft = 31;//fires left flipper green wire
const int buttonStart = 32;

//Scoreboard display Pins
const int dataPin = 4;  // yellow wire to 74HC595 pin 14, serial pin
const int latchPin = 5; // white wire to 74HC595 pin 12, RCLK
const int clockPin = 6; // green to 74HC595 pin 11, SRCLK
const int display1 = 7;//yellow wire for first seven segment display
const int display2 = 8;//blue wire for the second seven segment display
const int display3 = 9;//yellow wire for the third seven segment display
const int display4 = 10;//blue wire for the fourth seven segment display

//Lives proximity Sensor Pin
const int photDig = 2;//blue wire from output of opamp to 2
//const int photoresistor = A3;

//Lives LED's
int livesLEDs[] = {33,34,35};

enum States {START, PLAY, END};
int State;

//game variables
int lives;
bool livesFlag;
int points;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //below is 
  //pinMode(photoresistor, INPUT);
  pinMode(photDig, INPUT);

  //display pin setting
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(display1, OUTPUT);
  pinMode(display2, OUTPUT);
  pinMode(display3, OUTPUT);
  pinMode(display4, OUTPUT);

  //solenoid pin setting
  pinMode(solenoidBlueL, OUTPUT);
  pinMode(solenoidBlackM, OUTPUT);
  pinMode(solenoidBlueR, OUTPUT);
  pinMode(solenoidLeft, OUTPUT);
  pinMode(solenoidRight, OUTPUT);

  //bumper input setting
  pinMode(bumper1, INPUT);
  pinMode(bumper2, INPUT);
  pinMode(bumper3, INPUT);
  
  //button pin setting
  pinMode(buttonRight, INPUT);
  pinMode(buttonLeft, INPUT);

  pinMode(buttonStart, INPUT);

  //setting all the displays off
  digitalWrite(display1, HIGH);
  digitalWrite(display2, HIGH);
  digitalWrite(display3, HIGH);
  digitalWrite(display4, HIGH);

  //declaring default values
  lives = 3;
  points = 0;
  
  States State = START;//enum value which controls the state machine
  
  livesFlag = false;//declaration of boolean for detecting rising edge
 

}
/*
 * loop which contains the state machine which is what controls the states
 * 
 * 
 */
void loop() {
  Serial.println(State);
  switch(State)
  {
    /*
     * START state
     * simply waits for the start button to be pressed to start playing the game
     * 
     */
    case START:

      //if statements below run the state machine
      //Serial.println(digitalRead(buttonStart));
      lives = 3;
      //Serial.println(lives);
      if (digitalRead(buttonStart) ==  HIGH)
      {
        State = PLAY;//Starting the play when button is pressed
        lives = 3;//magic number
        points = 0;
        
      }
      else if(digitalRead(buttonStart) == LOW)
      {
        State = START;
      }
      //end of the state machine control
      
      //rewriting the led's back to off for the start state
      for(int i = 0; i < 3; i++)
      {
        digitalWrite(livesLEDs[i], LOW);
      }
      
    break;
    //end of the START state

    /*
     * Play state
     * used to run the various functions that run the system as a whole
     * 
     */
    case PLAY:
      Flipper();//code that operates the flipper
      Bumper();//function that operates all of the bumpers
      mainDisplay(points);//code that operates the display
      Lives();//running the lives checker
      //Serial.println(lives);

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

    /*
     * End state 
     * simply displays the final score for a few seconds and then goes back to 
     * play state
     */
    case END:
      mainDisplay(points);//displays the final score
      delay(5000);//displays the final score for 5 seconds
      State = START;
      break;
      //end of the END state
  }
  
}


/*
 * Lives function
 * scans from the IR proximity sensor and counts the number lof lives
 * 
 * 
 */
 
void Lives() 
{
  //Serial.println(lives);
  int readLives = digitalRead(photDig);//read from sensor
  
  //turning the lights on based on the number of lives remaining
  for(int i = 0; i < lives; i++)
  {
    digitalWrite(livesLEDs[i], HIGH);
  }
  
  
  //detecting a rising edge for the lives count
  if (readLives == HIGH && livesFlag == false)
  {
    lives = lives - 1;
    livesFlag = true;
  }
  else if(readLives == LOW)
  {
    livesFlag = false;
  }

  //code that runs the Lives LEDs
  
  
}


/*
 * Flipper function
 * simply fires the flipper when the user presses the respective button
 */
void Flipper()
{
  int Right = digitalRead(buttonRight);
  int Left = digitalRead(buttonLeft);

  
  Serial.println(Right);

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
    delay(30);
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

/*
 * code below is a mix of multiple functions that run the display
 * the only external input is the points which is fed into the display
 * function
 * 
 */


/**************************************************************************************
 * mainDisplay is the mother function that takes int and displays it on display
 * param - displayedNum is the int to be displayed on the seven segment display
 ***************************************************************************************/
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
  //sending the bits via spi protocol
  digitalWrite(displayNum, LOW);//turning display on
  
  digitalWrite(latchPin, LOW);  // prepare shift register for data
  shiftOut(dataPin, clockPin, LSBFIRST, eightBits); // send data
  digitalWrite(latchPin, HIGH); // update display
  
  delay(5);//delay for each seperate display
  
  digitalWrite(displayNum, HIGH); //turning display off before turning on next display
  
}


/*
 * function that turns the int value and converts to bits
 * uses case structure to conver into to bits
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
