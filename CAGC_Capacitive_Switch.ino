#include <Keyboard.h>
#include <CapPin.h>

/*
//This is code I use in my class Costumes as Game Controllers. 
This program is based on Adafruit's plushy Game Controller project tutorial. 
https://learn.adafruit.com/plush-game-controller/the-code
Uses this library: https://github.com/moderndevice/CapSense 
 */

const int ledPin =  7;      // the number of the LED pin on FLORA

CapPin cPin_10 = CapPin(10);    // read pin 10 (D10 on Flora)
CapPin capacitivePin = cPin_10;

char key = 'a';
// check http://arduino.cc/en/Reference/KeyboardModifiers for more info on unique keys
 
boolean currentPressed = false;
// Capactive touch threashhold, you might want to mess with this if you find its too
// sensitive or not sensitiva enough
#define THRESH 500
 
float smoothed =0;
void setup()
{
  Keyboard.begin();
  pinMode(ledPin, OUTPUT);   
}
 
 
void loop()                    
{ 
  for (int i=0;i<8;i++) {
    delay(1);
    long start = millis();
    long total =  capacitivePin.readPin(2000);
 
    // check if we are sensing that a finger is touching the pad 
    // and that it wasnt already pressed
    if ((total > THRESH) && (! currentPressed)) {
      currentPressed = true;
      digitalWrite(ledPin, HIGH);  
      Keyboard.press(key);
    }
    else if ((total <= THRESH) && (currentPressed)) {
      // key was released (no touch, and it was pressed before)   
      currentPressed = false;
      digitalWrite(ledPin, LOW);
      Keyboard.release(key);
    }
    
/*
    // simple lowpass filter to take out some of the jitter
    // change parameter (0 is min, .99 is max) or eliminate to suit
    smoothed= = smooth(total, .8, smoothed);   
 
    Serial.print(i); Serial.print(": ");
    Serial.print( millis() - start);      // time to execute in mS
    Serial.print("ms \t");
    Serial.print(total);                  // raw total
    Serial.print("\t->\t");
    Serial.println((int) smoothed);       // smoothed
*/
    delay(5);
  }
}
 
// simple lowpass filter
// requires recycling the output in the "smoothedVal" param
int smooth(int data, float filterVal, float smoothedVal){
 
  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .999999;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }
 
  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);
 
  return (int)smoothedVal;
}
