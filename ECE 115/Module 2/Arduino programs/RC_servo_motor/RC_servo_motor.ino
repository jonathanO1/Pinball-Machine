// Include the Servo library 
#include <Servo.h> 
// Declare the Servo pin 
int servoPin = 3; 
int potPin = A0;
// Create a servo object 
Servo Servo1; 
void setup() { 
   // We need to attach the servo to the used pin number 
   Servo1.attach(servoPin); 
   //pinMode(A0,'input'); 
   Serial.begin(9600);
}
void loop(){ 
   int val = analogRead(A0);
   //Serial.println();
   int finVal = map(0,7704, 0, 180);
   Serial.println(finVal);
   Servo1.write(finVal); 
   delay(1000); 
}
