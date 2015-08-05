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
#include "Task.h"
#include "WProgram.h"

//--------------------------------------------------------------
// Task
//  Constructs a new task with a function pointer to something
//  not in a class (C-style function pointer).
//--------------------------------------------------------------
Task::Task(void (*func)(void), unsigned int time, bool repeat)
{
  pcallback_ = func;
  functor_ = false;
  this->init(time, repeat);
}

//--------------------------------------------------------------
// Task
//  Constructs a new task with a function pointer to something
//  in a class (C++-style functor).
//--------------------------------------------------------------
Task::Task(Function* func, unsigned int time, bool repeat)
{
  fcallback_ = func;
  functor_ = true;
  this->init(time, repeat);
}

//--------------------------------------------------------------
// Init
//  Basic initializers for the Task constructor. Sets the 
//  age, priority, whether it repeats or not and some default
//  parameters.
//--------------------------------------------------------------
void
Task::init(unsigned int time, bool repeat)
{
  age = time;
  time_ = time;
  priority = millis() + time;   // make the priority (sorta) unique.
  repeat_ = repeat;
  running_ = false;
}
   
//-------------------------------------------------------------- 
// Exec
//  Execute the function, setting the state of the task to 
//  running so it doesn't get interrupted.
//--------------------------------------------------------------
void 
Task::exec() {
  running_ = true;
  if (functor_) {
    (*fcallback_)();
  } else {
    (*pcallback_)();
  }
  running_ = false;
}
