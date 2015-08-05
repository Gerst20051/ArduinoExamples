#include <kernel.h>
#include <queue.h>

/*
 * serialDemo
 *
 * Created: 4/18/2013 7:22:55 PM
 *  Author: dcstanc
 
    	Copyright (C) 2013 Colin Tan
    	
    	
    	This file is part of ArdOS.

    	ArdOS is free software: you can redistribute it and/or modify
    	it under the terms of the GNU Lesser General Public License as published by
    	the Free Software Foundation, either version 3 of the License, or
    	(at your option) any later version.

    	ArdOS is distributed in the hope that it will be useful,
    	but WITHOUT ANY WARRANTY; without even the implied warranty of
    	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    	GNU Lesser General Public License for more details.

    	You should have received a copy of the GNU Lesser General Public License
    	along with ArdOS.  If not, see <http://www.gnu.org/licenses/>.
 */ 


#define NUM_TASKS      3

/* In this program task 1 blinks the LED at a rate specified by delayVal, which is controlled by the ADC read by task 3. 
    If the value read is different from the previous value, task 3 sends the new value over a queue to task 2 which sends
    it via serial link to the PC. You can monitor the ADC value using the Arduino IDE's Serial Monitor */
  
   
// Message queue
int qbuf[16];
TMsgQ queue;

unsigned int delayVal=1000;

// Task 1 does the blinking
void task1(void *p)
{
  while(1)
  {
    digitalWrite(6, HIGH);
    OSSleep(delayVal);
    digitalWrite(6, LOW);
    OSSleep(delayVal);  
  }
}

// Task 2 reads from the queue

void task2(void *p)
{
  while(1)
  {
      unsigned int val=(unsigned int) OSDequeue(&queue);
      analogWrite(11, val);

      // Output to serial
      Serial.println(delayVal, DEC);
  }
}

void task3(void *p)
{
  unsigned int old_val=delayVal;
  
  while(1)
  {
      // Read in the value from the ADC every 250ms. Transmit to Task 2 only if the value actually changes.
      unsigned int val=analogRead(0);
      if(val!=old_val)
      {
        delayVal=val;
        OSEnqueue(val, &queue);
        old_val=val;
      }
      
      OSSleep(250);
  }
}

// Number of tasks we intend to create
#define NUM_TASKS  3
void setup()
{
  // Initialize ArdOStasks
  OSInit(NUM_TASKS);  
  pinMode(6, OUTPUT);
  Serial.begin(9600);
  
  // Create the queue
  OSMakeQueue(qbuf, 16, &queue);
  
  // Add in the tasks
  OSCreateTask(0, task1, NULL);
  OSCreateTask(1, task2, NULL);
  OSCreateTask(2, task3, NULL);
 // Start the OS
  OSRun();
  
}

void loop()
{
  // empty
}
