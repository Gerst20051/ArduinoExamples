#include <kernel.h>
#include <sema.h>


/*
 * swapDemo
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


TOSSema sem1, sem2;
#define NUM_TASKS  4

/* This program demonstrates the use of binary semaphores for coordinating tasks. It assumes that LEDs are conencted to pins 6 and 9 */

void task1(void *p)
{
	while(1)
	{
		// Flash 5 times
		
		for(unsigned char i=0; i<5; i++)
		{
			digitalWrite(6, LOW);
			OSSleep(125);
			digitalWrite(6, HIGH);
			OSSleep(125);			
		}
		
		digitalWrite(6, LOW);
		// Release semaphore
		OSGiveSema(&sem1);
		
		// Wait for sem2 to be released
		OSTakeSema(&sem2);
	}
}

void task2(void *p)
{
	while(1)
	{
		// Take semaphore
		OSTakeSema(&sem1);
		
		// Flash 5 times
		
		for(unsigned char i=0; i<5; i++)
		{
			digitalWrite(9, LOW);
			OSSleep(250);
			digitalWrite(9, HIGH);
			OSSleep(250);
		}

		digitalWrite(9, LOW);		
		// Give sema2
		OSGiveSema(&sem2);
	}
}

void task3(void *p)
{
	unsigned char val=0;
	while(1)
	{
		analogWrite(11, val);
		val=(val+1)%255;
		OSSleep(5);
	}
}

void task4(void *p)
{
  int count=(int) p;
  
  while(1)
  {
    Serial.print(count);
    Serial.print("\n");
    count++;
    OSSleep(500);
  }
}

void setup()
{
	// Arduino init
	OSInit(NUM_TASKS);
        
        // Set a larger stack size for all tasks
        setTaskStackSize(45);
        	
  	// Initialize two binary semaphores to 0.
	OSInitSema(&sem1, NUM_TASKS, 0, 1);
	OSInitSema(&sem2, NUM_TASKS, 0, 1);
	
	OSCreateTask(0, task1, NULL);
	OSCreateTask(1, task2, NULL);
	OSCreateTask(2, task3, NULL);
        
        // Fourth task writes values to the serial port every 500ms
        OSCreateTask(3, task4, (void *) 4);
        
        // Initialize the serial port and output pins
        Serial.begin(9600);
	pinMode(6, OUTPUT);
	pinMode(9, OUTPUT);
	OSRun();
}

void loop()
{
  // Empty
}
