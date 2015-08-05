// kernel.h has to be included in all ArdOS projects. We include mutex.h to use the mutex lock and conditional variables

#include <kernel.h>
#include <mutex.h>

/*
 * mutexDemo
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

unsigned int count=1;
OSCond c1, c2;
OSMutex mutex;

// The number of tasks we intend to create
#define NUM_TASKS      2

/* This program demonstrates the use of mutex locks and conditional variables. It assumes that LEDs are connected to pins 6 and 9 */

/* Task 1 causes LED at pin 6 to "count" times, increments count, hands over control to Task 2 which flashes LED at pin 9  "count" times,
   increments count by 2, hands control back Task 1, etc. */
   
void task1(void *p)
{
	while(1)
	{
		OSTakeMutex(&mutex);
		// Flash 5 times
		
		for(unsigned int i=0; i<count; i++)
		{
			digitalWrite(6, LOW);
			OSSleep(125);
			digitalWrite(6, HIGH);
			OSSleep(125);			
		}
		
		digitalWrite(6, LOW);
		count++;
		OSSignal(&c2);
		OSWait(&c1, &mutex);
		OSGiveMutex(&mutex);
		OSSleep(1);
	}
}

void task2(void *p)
{
	while(1)
	{
		OSTakeMutex(&mutex);
		OSWait(&c2, &mutex);
		
		for(unsigned char i=0; i<count; i++)
		{
			digitalWrite(9, LOW);
			OSSleep(250);
			digitalWrite(9, HIGH);
			OSSleep(250);
		}

		digitalWrite(9, LOW);
		count+=2;
		OSSignal(&c1);
		OSGiveMutex(&mutex);
	}
}

void setup()
{
        // Initialize the OS telling it how many tasks we want to create
	OSInit(NUM_TASKS);
	
	// Create the mutexes and conditional variables.
	OSCreateMutex(&mutex, NUM_TASKS);
        OSCreateConditional(&c1);
	OSCreateConditional(&c2);
		
        // Create our two tasks
	OSCreateTask(0, task1, NULL);
	OSCreateTask(1, task2, NULL);

        // Configure pins 6 and 9 as output
	pinMode(6, OUTPUT);
	pinMode(9, OUTPUT);

        // And start the OS
	OSRun();
}

void loop()
{
  // Empty
}
