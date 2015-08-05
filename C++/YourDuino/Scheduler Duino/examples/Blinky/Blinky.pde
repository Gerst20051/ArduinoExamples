//--------------------------------------------------------------
// Blinky
// ======
//  Shows example usage of scheduling functions.
//--------------------------------------------------------------
// Copyright (c) 2010 the artist authors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//--------------------------------------------------------------
// Contributors:
//   Tim Evans <tevans01@syr.edu>
//--------------------------------------------------------------
#include <Scheduler.h>

#define LED_PIN 13       // change to where you have your led!

// The scheduler.
Scheduler sched = Scheduler();

boolean ledOn = false;   // is the led on?

// Blink the LED!
void blink() {
  if (ledOn) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH); 
  }
  ledOn = ~ledOn;
}

void setup() {
  // Schedule a new item and retrieve the task so it can be killed later.
  Task* blinker = sched.setInterval(blink, 500);
  
  // Wait 10 seconds
  delay(10000);
  
  // Shut it down!
  sched.clearInterval(blinker);
  
  // One more...
  sched.setTimeout(blink, 1000);
}

void loop() { }

