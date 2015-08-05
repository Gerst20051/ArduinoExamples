//--------------------------------------------------------------
// Task
// ====
//  Describes a single task in the scheduler.
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
#ifndef ARDUINO_SCHEDULER_TASK
#define ARDUINO_SCHEDULER_TASK

#include "Function.h"

//--------------------------------------------------------------
// Task
// ====
//  A task to perform in the future. Used as a bundle to
// retrieve queued actions.
//--------------------------------------------------------------
class Task {
  public:
    // Create a new task with default parameters.
    Task(void (*func)(void), unsigned int time, bool repeat);
    Task(Function* func, unsigned int time, bool repeat);
    
    // Execute the function, making sure preemption does not occur.
    void exec();
    
    // Returns whether the task is running or not
    bool running() { return running_; }
    
    // Returns the lifetime of the task
    int lifetime() { return time_; }
    
    // Returns whether the task repeats
    bool repeats() { return repeat_; }
    
    // The age of the task (time left until execution.)
    long age;
    
    // The priority of the task- used to remove items
    int priority;
    
  private:
    // The lifetime of the Task
    int time_;
    
    // Whether the Task is repeating (an interval)
    bool repeat_;
    // Whether the Task is running
    bool running_;
    // Whether the Task is a functor (or function pointer)
    bool functor_;
    
    // Function pointer callback
    void (*pcallback_)(void);
    // Functor pointer callback
    Function* fcallback_;
    
    void init(unsigned int time, bool repeat);
};

#endif // ARDUINO_SCHEDULER_TASK
