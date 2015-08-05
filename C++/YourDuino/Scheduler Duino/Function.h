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
#ifndef ARDUINO_VOID_FUNCTION_PTR
#define ARDUINO_VOID_FUNCTION_PTR

//--------------------------------------------------------------
// Function
// ========
//  Abstract class to a void function pointer.
//--------------------------------------------------------------
class Function {
  public:
    virtual void operator()(void)=0;
};

//--------------------------------------------------------------
// VoidFxPtr
// =========
//  void function pointer to a function inside of a class.
//--------------------------------------------------------------
template <class T>
class VoidFxPtr : public Function {
  private:
    void (T::*func_)();
    T* object_;
 
  public:
    VoidFxPtr(T* object, void(T::*func)()) {
      object_ = object;
      func_ = func;
    };
    
    virtual void operator()() {
      (*object_.*func_)();
    };
};

#endif // ARDUINO_VOID_FUNCTION_PTR
