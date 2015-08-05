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
#ifndef ARDUINO_SCHEDULER
#define ARDUINO_SCHEDULER

#include "WProgram.h"
#include "Task.h"

//--------------------------------------------------------------
// Scheduler
// =========
//  Provides simplistic scheduling for functions, based off
// of the Javascript ones.
//--------------------------------------------------------------
class Scheduler {
  public:
    // Constructor. Set up variables.
    Scheduler();
    
    // Call func once after the specified msec
    Task* setTimeout(void (*func)(void), unsigned int msec);
    Task* setTimeout(Function* func, unsigned int msec);
    
    // Call func every msec milliseconds
    Task* setInterval(void (*func)(void), unsigned int msec);
    Task* setInterval(Function* func, unsigned int msec);
    
    // Stop the Task from happening.
    void clearTimeout(Task* to_kill);
    
    // Stop the Task from happening (anymore).
    void clearInterval(Task* to_kill);
};

#endif // ARDUINO_SCHEDULER
