/*  
    Motion sensor for Arduino, example code
    Tutorial:  http://www.hacktronics.com/Tutorials/arduino-motion-sensor.html
    Copyright: Mark McComb, hacktronics LLC
    License: http://www.opensource.org/licenses/mit-license.php (Go crazy)
*/

const byte ledPin =  13;    // LED pin
const byte motionPin = 2;   // motion detector input pin 
byte senseMotion = 0;       // variable to hold current state of motion detector

void setup() {
  // set the digital pin directions
  pinMode(ledPin, OUTPUT);      
  pinMode(motionPin, INPUT);
}

void loop()
{
    // Now watch for burglers
    senseMotion = digitalRead(motionPin);
    if (senseMotion == HIGH) {    // burgler found!
      digitalWrite(ledPin, HIGH);
    } else {                      // no burgler, yet...
      digitalWrite(ledPin, LOW);
    }
}

