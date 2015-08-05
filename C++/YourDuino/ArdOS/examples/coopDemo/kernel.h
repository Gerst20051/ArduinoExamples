/*
 * kernel.h
 *
 * Created: 13/4/2013 10:13:52 AM
 *  Author: dcstanc
 *
 * kernel.c - Kernel main routines
 *
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


#ifndef KERNEL_H_
#define KERNEL_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>


/* ========================================== ArdOS Configuration START ========================================================*/
 
/* ------------------------------------------------------------------------

Operating System Configuration Section

This set of parameters is used to configure the Operating System

---------------------------------------------------------------------------- */

// OS Scheduler Types
#define OS_PRIORITY	0
#define OS_RR		1
#define OS_RMS		2
#define OS_EDF		3

// Supported CPU Types
#define AT168		0
#define AT328		1
#define AT1280		2
#define AT2560		3

/* Configure the CPU Type
 Valid values are AT168, AT328, AT1280 and AT2560. Normally set by the Arduino IDE.
*/

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
	#define OSCPU_TYPE	AT2560
#else
	#define OSCPU_TYPE	AT328
#endif

/* Configuring the Round Robin Time Quanta */

#define OSMIN_QUANTUM		5		// Minimum execution time in ms

#define OSMAX_QUANTUM		500		// Maximum execution time in ms
#define OSMAX_PRIOLEVEL		3		// Highest priority level

#define OSQUANTUM_FACTOR	(OSMAX_QUANTUM-OSMIN_QUANTUM)/OSMAX_PRIOLEVEL

/* Sets whether ArdOS operates in pre-emptive or co-operative mode */

#define OS_PREEMPTIVE		0

/* Scheduler type: 0 = Fixed priority, 1 = Priority RR, 2 = EDF */

#define OSSCHED_TYPE		OS_PRIORITY

/*  Parameters for the task manager */

// Task stack size
#define OS_STACKLEN		20

// Use sleep
#define OSUSE_SLEEP			1

/* 
	The following parameters control whether to generate code for the additional ArdOS features:
	
	Semaphores
	FIFO Message Queues
	Priority Message Queues
	Mutex Locks
	Conditional Variables
*/

// Use semaphores
#define OSUSE_SEMA			1

// Use queues
#define OSUSE_QUEUES		0

// Use priority queues
#define OSUSE_PRIOQUEUES	0

// Mutexes and conditionals
#define OSUSE_MUTEXES		0

#define OSUSE_CONDITIONALS	0

/* ========================================== ArdOS Configuration END ========================================================*/

/* ------------------------------------------------------------------------

Kernel Variables Section

External hooks for kernel variables.

---------------------------------------------------------------------------- */

typedef struct tc
{
	uint8_t pid;	// Process ID
	uint8_t prio;	// Task priority
	
	// For round robin scheduler
	#if OSSCHED_TYPE==OS_RR
	unsigned int quantum;
	unsigned int timeleft;
	#endif
	
	unsigned char status; // bit 0 = first run flag, bit 1 = blocked flag
	unsigned long stack[OS_STACKLEN];
	unsigned long sp;	// Stack pointer
	void (*taskptr)(void*); // Task pointer
	void *rarg;
} tTCB;


// Definition for task queues
typedef struct tq
{
	unsigned char *qptr, head, tail, len, ctr;
} tQueue;

#if OS_DEBUG == 1
void printProcQ(tQueue *q, tTCB *tasklist);
#endif

void initQ(unsigned char *, unsigned char len, tQueue *q);
void prioEnq(int pid, tTCB *tasklist, tQueue *q);
void enq(int pid, tQueue *q);
void procEnq(int pid, tTCB *tasklist, tQueue *q);
unsigned char procPeek(tQueue *q);
unsigned char procDeq(tQueue *q);


/* ------------------------------------------------------------------------

Kernel Variables Section

External hooks for kernel variables.

---------------------------------------------------------------------------- */

// Number of user tasks, maximum number of tasks

extern unsigned char _procCount;
extern tTCB *_tasks;

// Task Ready Queue
extern tQueue _ready;

// Task running index
extern unsigned char _running;

// Task Status flags
#define _OS_FIRSTRUN	0b1
#define _OS_BLOCKED		0b10


// Error codes
#define OS_NO_ERR			0	// No error
#define OS_ERR_MAX_PROCS	1	// Reached maximum processes		
#define OS_ERR_BAD_PRIO		2	// Bad priority number (RR)
#define OS_ERR_DUP_PRIO		3	// Duplicate priority number (Fixed Priority)

/* ----------------------------------------------------------------------------------------

ArdOS - Private OS Routines Section

This section contains private OS functions that should not be called by user programs.

 ------------------------------------------------------------------------------------------- */

// Create atomic sections of code by disabling interrupts
void OSMakeAtomic();
void OSExitAtomic();

// Error routines
unsigned int OSGetError();
void OSSetError(unsigned int);

/*  ----------------------------------------------------------------------------

ArdOS - Public OS Routines Section

These routines are for user programs to call.

-------------------------------------------------------------------------------- */

/* Initialize the operating system. 

	Pre: numTasks = Number of tasks the programmer intends to create.
	Post: The operating system internals are initialized.
	
	All ArdOS calls are valid only if OSInit has already been called.
	
*/

void OSInit(unsigned char numTasks);

/* Starts the operating system.

	Pre: OSInit has been called with the number of tasks the user is creating.
	Post: The operating system is started and the highest priority task starts executing. This function never exits.
	
*/

void OSRun();

/* Create a new task

	Pre: OSInit has been called with the number of tasks the user is creating.
		 prio = Task priority. 0 is highest, numTasks-1 is lowest.
		 rptr = Pointer to the user's task, written as a function with prototype void fun(void *)
		 rarg = Arguments to be passed to the task on start-up. Put NULL if no arguments are to be passed
*/

unsigned int OSCreateTask(int prio, void (*rptr)(void *), void *rarg);

/* Calls the task swapper to surrender control of the CPU. Used only in co-operative mode

	Pre: ArdOS has been configured for co-operative operation.
	Post: Control is handed to the next ready task. Control is handed back to the calling task if no other task is ready */

void OSSwap() __attribute__ ((naked));

/* Calls the task swapper to surrender control of the CPU. Used only in co-operative mode

	Pre: ArdOS has been configured for co-operative operation.
	Post: Control is handed to the next ready task IF it has a higher priority than the calling task.
		If there are no higher priority tasks ready, control is handed back to the calling task.
*/

void OSPrioSwap() __attribute__ ((naked));

/* Set the idle hook

	When all tasks are blocked, ArdOS would normally call a dummy tasks that burns through excess CPU cycles using
	a busy wait. Use this to set the idle hook to execute one of your own idle task. Your idle task should not
	call any blocking functions.
	
	Pre: idletask = Pointer to your idle task in the form of a function of prototype void fun(void *)
	Post: Idle task hook is set. Your task will be called when there are no other ready tasks.
*/

void OSSetIdleTask(void (*idletask)(void *));

/* Puts a task to sleep for the specified time

	This function causes the calling task to sleep for at least the number of milliseconds specified.
	
	Pre: millis = Number of milliseconds to sleep
	Post: Task is placed into the blocked state and becomes un-runnable until millis milliseconds have passed.
	
*/
void OSSleep(unsigned int millis);

/* Returns the number of ticks since OSRun was called. 

This function returns the number of clock ticks since ArdOS was started using OSRun. Normally this would be equivalent to
the number of milliseconds passed. This value over to 0 approximately once every 49.7 days 

*/

unsigned long OSticks();


/*
	ISR routines: These routines are called only from within interrupt service routines
*/

/* Suspends and resumes the scheduler

	This pair of routines should be called if your ISR makes unblocking calls (e.g. OSGiveSema or OSEnqueue) to prevent the
	ISR from being inadvertently pre-empted.
	
	Usage:
	
	ISR(isr_vector)
	{
		OSSuspendscheduler();
		...
		OSGiveSema(&sem1);
		...
		OSResumeScheduler();
	}		
*/

/*  Suspend the scheduler

	Pre: An ISR is currently in process and intends to make an unblocking call
	Post: The scheduler is suspended.
*/
void OSSuspendScheduler();

/* Restarts the scheduler
	Pre: The ISR has completed processing and can exit.
	Post: The scheduler is restarted
*/
void OSResumeScheduler();

/* The following pair of routines are the ISR equivalents of OSSwap and OSPrioSwap. You should not normally need
   to use these */

void OSSwapFromISR() __attribute__ ((naked));
void OSPrioSwapFromISR() __attribute__ ((naked));

#endif /* KERNEL_H_ */