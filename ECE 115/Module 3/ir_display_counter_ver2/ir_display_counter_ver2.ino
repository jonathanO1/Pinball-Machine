/*******************************************************************
 * Ver 2
 * changes in this program in moving all of the main functionality into
 * separate functions to enable modularity and to make larger integration 
 * possible
 * 
 */


//Global pin values
const int photoresistor = A3;//blue jumper form phototransistor to A3
const int photDig = 2;//green wire from output of opamp to 2
const int dataPin = 4;  // blue wire to 74HC595 pin 14, serial pin
const int latchPin = 5; // green to 74HC595 pin 12, RCLK
const int clockPin = 6; // yellow to 74HC595 pin 11, SRCLK
const int display1 = 7;//yellow wire for first seven segment display
const int display2 = 8;//blue wire for the second seven segment display
const int display3 = 9;//yellow wire for the third seven segment display
const int display4 = 10;//blue wire for the fourth seven segment display


//variables for counting the number of times object passes the sensor
int count;//maybe change to lives later or use sensor for a point system
bool countFlag;//declare to keep track of the ir sensor


/*
 * Setup: turing on serial and setting the pinModes, turning the displays
 * off and setting the counting variables to off
 */
void setup() 
{
  //setting pins up and begginning the serial
  Serial.begin(9600);
  pinMode(photoresistor, INPUT);
  pinMode(photDig, INPUT);
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
  
  countFlag = false;
  count = 0;
}



/*
 * Loop function: reads the input runs the count function for 
 * counting the number of times object passes sensor
 * 
 */
void loop() 
{
  
  //int val = analogRead(photoresistor);//testing the analog read over the phototransistor
  int digRead = digitalRead(photDig);//reading the digital values from the comparator

  countFlag = irCount(digRead, countFlag);//couting the number of times sensor goes off
  //turn this code below into a function later for use on a larger scale

}


/*
 * fuction takes in the digital val and flag to keep track of last val
 * digRead: the digital val from sensor and aanalog-digital
 * ifFlag: flag that tracks the last value to detect change in value
 */
bool irCount(int digRead, bool irFlag)
{
  //using flag to keep track of past value so only counts once
  if (digRead == HIGH && irFlag == false)
  {
    count = count + 1;
    irFlag = true;//flag keeps track of the last value to detect change
  }
  else if(digRead == LOW)
  {
    irFlag = false;
  }
  
  mainDisplay(count);//calling the display to display the count 
  
  return irFlag;//returing the flag so that it can loop and count correctly
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
