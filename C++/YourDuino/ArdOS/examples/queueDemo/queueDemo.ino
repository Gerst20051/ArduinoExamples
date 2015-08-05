/*
 * queueDemo
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

#include <kernel.h>
#include <queue.h>

TMsgQ q1, q2;

void task1(void *p)
{
	int count=1;
	
	while(1)
	{
		// Flash 5 times
		
		for(unsigned char i=0; i<count; i++)
		{
			digitalWrite(6, LOW);
			OSSleep(125);
			digitalWrite(6, HIGH);
			OSSleep(125);			
		}
		
		digitalWrite(6, LOW);
		count++;
		OSEnqueue(count, &q1);
		count=OSDequeue(&q2);
	}
}

void task2(void *p)
{
	int count;
	
	while(1)
	{
		count=OSDequeue(&q1);
		for(unsigned char i=0; i<count; i++)
		{
			digitalWrite(9, LOW);
			OSSleep(250);
			digitalWrite(9, HIGH);
			OSSleep(250);
		}

		digitalWrite(9, LOW);
		count+=2;
		OSEnqueue(count, &q2);		
	}
}

int qbuff1[8], qbuff2[8];

void setup()
{
	OSInit(2);
	
        setTaskStackSize(120);
	// Create the queue
	OSMakeQueue(qbuff1, 8, &q1);
	OSMakeQueue(qbuff2, 8, &q2);
	OSCreateTask(0, task1, NULL);
	OSCreateTask(1, task2, NULL);
        Serial.begin(9600);
	pinMode(6, OUTPUT);
	pinMode(9, OUTPUT);
	OSRun();
}

void loop()
{
}

