//--------------------------------------------------------------
// Scheduler
// =========
//  Provides non-blocking scheduling routines for the
// Arduino platform. 
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
#include "Scheduler.h"
#include "util/List.h"

#define ISR_FREQ_MS 124             // Frequency in Hz = 125,000 / (ISR_FREQ + 1)

//--------------------------------------------------------------
// C++ goodies!
//  avr-g++ does not support new and delete at this time,
//  so here they are!
//--------------------------------------------------------------
void*
operator new(size_t size) 
{ 
  return malloc(size); 
} 

void
operator delete(void * ptr)
{ 
  free(ptr); 
}

List<Task*>* ready_queue;           // A ready queue of scheduled tasks
Task* running;                      // Task using the CPU.
Task* reaper;                       // Task to have it's memory harvested.

//--------------------------------------------------------------
// Map Age
//  Age the tasks Benjamin Button style so no data is lost
//  thanks to potential overflows. Called by List->map.
//  Input:
//    task  | an item in the ready queue.
//  Usage:
//    List<Task*>* ready_queue = new List<Task*>();
//    ready_queue->insert(new Task(...));
//    ready_queue->map(map_age);
//--------------------------------------------------------------
void
map_age(Task* task)
{
  task->age--;
}

//--------------------------------------------------------------
// Enable
//  Change Output Compare Interrupt Enable to 1
//--------------------------------------------------------------
void
enable()
{
  // Turn on interrupts
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__)
  TIMSK2 |= 1 << OCIE2A;
#else
  TIMSK |= 1 << OCIE2;
#endif
}

//--------------------------------------------------------------
// Disable
//  Change Output Compare Interrupt Enable to 0
//--------------------------------------------------------------
void
disable()
{
  // Turn off interrupts
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__)
  TIMSK2 &= ~(1 << OCIE2A);
#else
  TIMSK &= ~(1 << OCIE2);
#endif
}

//--------------------------------------------------------------
// Timer 2 interrupt
//  Called when Timer 2 interrupts. Finds tasks to execute,
//  and execute them.
//--------------------------------------------------------------
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__)
ISR(TIMER2_COMPA_vect)
#else
ISR(TIMER2_COMP_vect)
#endif
{
  // No interrupts in here!  
  uint8_t old_sreg = SREG;
  cli();
  
  TCNT2 = 0;
  
  // Check if something's still running
  if (running != NULL && !running->running()) {
    if (running->repeats()) {                       // If it repeats
      running->age = running->lifetime();             // reset it and
      running->priority = millis() + running->lifetime();
      ready_queue->insert(running, running->priority);// put it back in the ready queue
      
    } else {                                          // Otherwise
      reaper = running;                                 // Set it to be reaped.
    }
    running = NULL;                                   // Free up the running spot
  }
  
  ready_queue->map(map_age);                        // Age everybody
  
  Task* tmp = ready_queue->poll();                  // Get the head.
  if (tmp->age <= 0) {                              // If it should run,
    running = tmp;                                    // schedule it to run
  } else {                                          // Otherwise,
    ready_queue->insert(tmp, tmp->priority);          // put it back in the ready queue.
  }
  
  SREG = old_sreg;
  // Non-interruptable stuff done!
   
  if (running != NULL && !running->running()) {     // If there's something to be ran,
    running->exec();                                  // run it!
  }

  if (reaper != NULL) {                             // If there's something ready to die,
    delete reaper;                                    // reap it!
    reaper = NULL;
  }
  
  // Disable this interrupt if nothing else is scheduled.
  if (ready_queue->is_empty() && running == NULL)
    disable();
}

//--------------------------------------------------------------
// Scheduler constructor
//  Initialize Timer 2 to take care of all of the scheduling.
//--------------------------------------------------------------
Scheduler::Scheduler()
{
#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega1280__)
  TCCR2A = (1 << WGM21);              // CTC
  TCCR2B = (1 << CS22) | (1 << CS20); // set prescaler to 125kHz
  TCCR2B &= ~(1 << CS21);
  OCR2A = ISR_FREQ_MS;
#else
  // Use normal mode
  TCCR2 = (1 << WGM21);               // CTC
  TCCR2 |= (1 << CS22) | (1 << CS20); // set prescaler to 125kHz
  TCCR2 &= ~(1 << CS21);
  OCR2 = ISR_FREQ_MS;
#endif
  TCNT2 = 0;
  ready_queue = new List<Task*>();
  running = NULL;
  reaper = NULL;
  
  sei();                            // Enable interrupts  
}

//--------------------------------------------------------------
// Set Timeout
//  Javascript-y function that calls a function after a 
//  specified delay. Not guaranteed to happen exactly at
//  that time, only approximately!
//  Input:
//    func    | the function scheduled to be called in the future
//    msec    | the requested delay before calling func
//--------------------------------------------------------------
Task*
Scheduler::setTimeout(void (*func)(void), unsigned int msec)
{
  if (msec == 0) {                        // don't wait!
    (*func)();                              // just call the function.
  }
  
  Task* t = new Task(func, msec, false);  // create a new task
  ready_queue->insert(t, t->priority);    // insert it in the proper location
  
  // Turn on interrupts
  enable();
  return t;
}

Task*
Scheduler::setTimeout(Function* func, unsigned int msec)
{
  if (msec == 0) {                        // don't wait!
    (*func)();                              // just call the function.
  }
  
  Task* t = new Task(func, msec, false);  // create a new task
  ready_queue->insert(t, t->priority);    // insert it in the proper location
  
  // Turn on interrupts
  enable();
  return t;
}

//--------------------------------------------------------------
// Set Interval
//  Javascript-y function that repeatedly calls a function 
//  after a specified interval. Not guaranteed to happen
//  exactly at that time, only approximately!
//  Input:
//    func    | the function scheduled to be called in the future
//    msec    | the requested period to call func
//--------------------------------------------------------------
Task*
Scheduler::setInterval(void (*func)(void), unsigned int msec)
{
  if (msec == 0) {                        // don't wait!
    (*func)();                              // just call the function.
  }
  
  Task* t = new Task(func, msec, true);   // create a new repeating task
  ready_queue->insert(t, t->priority);    // insert it in the proper location
  
  // Turn on interrupts
  enable();
  return t;
}

Task*
Scheduler::setInterval(Function* func, unsigned int msec)
{
  if (msec == 0) {                        // don't wait!
    (*func)();                              // just call the function.
  }
  
  Task* t = new Task(func, msec, true);   // create a new repeating task
  ready_queue->insert(t, t->priority);    // insert it in the proper location
  
  // Turn on interrupts
  enable();
  return t;
}

//--------------------------------------------------------------
// Clear Timeout
//  Javascript-y function to stop a timeout from happening.
//    to_kill   | the task to stop
//--------------------------------------------------------------
void
Scheduler::clearTimeout(Task* to_kill)
{
  // No interrupts in here!  
  uint8_t old_sreg = SREG;
  cli();
  
  // Remove the task from the ready queue.
  delete ready_queue->remove(to_kill->priority);
  
  SREG = old_sreg;
}

//--------------------------------------------------------------
// Clear Interval
//  Javascript-y function that stops a task from happening
//  anymore.
//  Input:
//    to_kill   | the task to stop
//--------------------------------------------------------------
void
Scheduler::clearInterval(Task* to_kill)
{
  this->clearTimeout(to_kill);
}
