
/*
 * coopSwap
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


/* This program demonstrates the use of co-operative multitasking. It assumes that LEDs are conencted to pins 6, 9 and 11.

   To customize your copy of ArdOS for a particular application, you should copy kernel.h from your library directory 
   
   (On Windows that would be in "Documents\Arduino\libraries\ArdOS" or "My Documents\Arduino\libraries\ArdOS". On Macs it would
    usually be in "Documents/Arduino/libraries/ArdOS")
    
    to your sketch directory. You should change the #include from:
    
    #include <kernel.h>
    
    to:
    
    #include "kernel.h"
    
    You can then freely customize the kernel.h file in your sketch directory.
    
    If you modify the kernel.h in Documents/Arduino/libraries/ArdOS the changes will affect all other sketches that use ArdOS */
    
    
// Include the customized kernel.h with preemptive turned off (OS_PREEMPTIVE set to 0)
#include "kernel.h"

// We have not included any other ArdOS headers as we are using only the OSSwap function which is defined in kernel.h. We define 3 tasks
#define NUM_TASKS  3

void task1(void *p)
{
	while(1)
	{
	      digitalWrite(6, LOW);
	      OSSleep(125);
	      digitalWrite(6, HIGH);
	      OSSleep(125);		

              // Hand control over to the next task
              OSSwap();	
	}
}

void task2(void *p)
{
	while(1)
	{
		digitalWrite(9, LOW);
		OSSleep(250);
		digitalWrite(9, HIGH);
		OSSleep(250);

              // Hand control over to the next task
                OSSwap();
	}
}

void task3(void *p)
{
	unsigned char val=0;
	while(1)
	{
		analogWrite(11, val);
		val=(val+1)%255;
            // Hand control to the next task
                OSSleep(20);
                OSSwap();
	}
}

void setup()
{
        /* Initialize ArdOS with 3 tasks */
        
        OSInit(NUM_TASKS);
        
        // Configure pins 6 and 9. Pin 11 is used with PWM so no need to initialize here.
	pinMode(6, OUTPUT);
	pinMode(9, OUTPUT);	

        // Create the 3 tasks
	OSCreateTask(0, task1, NULL);
	OSCreateTask(1, task2, NULL);
	OSCreateTask(2, task3, NULL);

        // Start the OS
	OSRun();
}

void loop()
{
  // Empty
}
