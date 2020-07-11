
// Global pin values

const int dataPin = 4;  // blue wire to 74HC595 pin 14, serial pin
const int latchPin = 5; // green to 74HC595 pin 12, RCLK
const int clockPin = 6; // yellow to 74HC595 pin 11, SRCLK
const int display1 = 7;//yellow wire for first seven segment display
const int display2 = 8;//blue wire for the second seven segment display
const int display3 = 9;//yellow wire for the third seven segment display
const int display4 = 10;//blue wire for the fourth seven segment display

//uncomment one of the following lines that describes your display
 // and comment out the line that does not describe your display 
 
const char common = 'a';    // common anode
//const char common = 'c';    // common cathode

//bool decPt = true;  // decimal point display flag
 
void setup() {
  Serial.begin(9600);
  // initialize I/O pins
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
}


void loop() {
  /*
  int inputNum = 5678;//hardcoded placeholder, will later put other values here

  //putting the value into an array of four digits
  int arr[4];//holds the value so each digit can be processed individually
  for (int i = 0; i < 4; i++)
  {
    int digit = inputNum % 10;
    inputNum /= 10;//reducing the sie of the num
    arr[3-i] = digit;//put the number into the array
  }
  
  for(int i = 0; i < 4; i++)
  {
    byte num = myfnNumToBits(arr[i]);
    int numdisplay = i + 7;
    Serial.println(arr[i]);
    myfnUpdateDisplay(num, numdisplay);
    
  }
  */
 
  
  
  
  byte bits = myfnNumToBits(8) ; //test line for hardcoded values
  
  myfnUpdateDisplay(bits, display4);    // display alphanumeric digit
;                 // pause   
}

//sending a digit to the display
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
 * function that turns the number into the bits to display
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
      return B11011001;//works
      break;
    case 8:
      return B00001000;//works
      break;
    case 9:
      return B10001000;//done
      break;
    
  }
}
