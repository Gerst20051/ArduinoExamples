//--------------------------------------------------------------
// Linked List Iterator
// ====================
//  Iterator over a linked list.
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
#ifndef ARDUINO_LINKED_LIST_ITERATOR
#define ARDUINO_LINKED_LIST_ITERATOR

#include "List.h"
#include "Node.h"

//--------------------------------------------------------------
// Iterator
// ========
//  Returns an iterator over the LinkedList.
//--------------------------------------------------------------
template <class T>
class Iterator {
 public:
  // Iterator constructor
  Iterator(Node<T>* head);
  // Return the element and increase the current position  
  T next();
  // Does the next element exist?
  bool has_next();
 private:
  Node<T>* current_;
};


//--------------------------------------------------------------
// Iterator constructor
//  Creates a new iterator by pointing to the first item in
// the sequence.
//  Input:
//    head      | the pointer to the first element of the list
//--------------------------------------------------------------
template <class T>
Iterator<T>::Iterator(Node<T>* head)
{
  current_ = head;
}

//--------------------------------------------------------------
// Next
//  Returns the current element in the list and moves the
// pointer to the next element.
//--------------------------------------------------------------
template <class T>
T
Iterator<T>::next()
{
  T data = current_->data;
  current_ = current_->next;
  return data;
}

//--------------------------------------------------------------
// Has Next
//  Returns true if the next element exists, otherwise it 
// returns false.
//--------------------------------------------------------------
template <class T>
bool
Iterator<T>::has_next()
{
  return !(current_->next == NULL);
}

#endif  // ARDUINO_LINKED_LIST_ITERATOR
