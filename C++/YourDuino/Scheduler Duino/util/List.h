//--------------------------------------------------------------
// Linked List
// ===========
//  LISP-like basic linked list for the Arduino platform.
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
#ifndef ARDUINO_LINKED_LIST
#define ARDUINO_LINKED_LIST

#include "Iterator.h"
#include "Node.h"

extern "C" {
  #include <stdlib.h>
  #include <stddef.h>
}

template <class T> class Iterator; 

//--------------------------------------------------------------
// List
// ====
//  Defines a singly linked list of nodes, where each points
//  to a single item on the list.
//--------------------------------------------------------------
template <class T>
class List {
 public:
  // List constructor
  List();
  
  // List destructor
  ~List();
  
  // LISP-like map function that performs the function
  // on each element in the list
  void map(void (*func)(T));
    
  // LISP-like reduce function that performs a function
  // on each element in the list with an initializer
  template <class R>
  R reduce(R (*func)(T, R), R init);
    
  // Returns true if the list is empty.
  bool is_empty();

  // Add an element to the end of the list
  void append(T item);

  // Add an element to the beginning of the list
  void prepend(T item);

  // Insert an item to its proper location in the list
  // according to its key
  void insert(T item, int key);

  // Remove an element from the beginning of the list
  T poll();

  // Remove an element from the end of the list
  T pop();

  // Remove the first element from the list with a matching key
  T remove(int key);
    
  // Return the size of the list
  int size() { return size_; }

  // Return the head of the list
  Iterator<T> iterator();
    
 private:
  // The head and tail of the list
  Node<T> *head_, *tail_;

  // The current size of the list
  int size_;
};


//--------------------------------------------------------------
// List constructor
//  Creates an empty list.
//--------------------------------------------------------------
template <class T>
List<T>::List()
{
  head_ = NULL;
  tail_ = NULL;
  size_ = 0;
}

//--------------------------------------------------------------
// List destructor
//  Removes all elements in the list.
//--------------------------------------------------------------
template <class T>
List<T>::~List()
{
  while (!this->is_empty()) {
    this->pop();
  }
  size_ = -1;
}

//--------------------------------------------------------------
// Map
//  LISP-like map function.
//  Input:
//    MapFxPtr func | The function to apply to each item on
//                  | the list
//  Usage:
//    void
//    print(int item) {
//      printf("%d, ", num);
//    }
//
//    int
//    main(int argc, char*[] argv) {
//      List<int>* lst = new List<int>();
//      lst->append(0);
//      lst->append(4);
//      lst->map(print);
//    }
//
//    Output: 0, 4, 
//--------------------------------------------------------------
template <class T>
void
List<T>::map(void (*func)(T))
{
  Node<T>* ptr = head_;
  for (ptr; ptr != NULL; ptr = ptr->next) {
    (*func)(ptr->data);
  }
}

//--------------------------------------------------------------
// Reduce
//  LISP-like reduce function.
//  Input:
//    R                | the template type for return values
//    ReduceFxPtr func | The function to apply to each item on
//                     | the list
//    int init         | The initializer for the currying
//                     | result.
//  Usage:
//    int
//    add(int item, int curried)
//    {
//      printf("%d + ", item);
//      return curried + item;
//    }
//    
//    int
//    main(int argc, char*[] argv)
//    {
//      List<int>* lst = new List<int>();
//      lst->append(4);
//      lst->append(5);
//      lst->append(1);
//      int sum = lst->reduce<int>(add, 0);
//      printf(" = %d", sum);
//    }
//
//    Output: 4, 5, 1, = 10
//--------------------------------------------------------------
template <class T>
template <class R>
R
List<T>::reduce(R (*func)(T, R), R init)
{
  R curried = init;
  Node<T>* ptr = head_;

  for (ptr; ptr != NULL; ptr = ptr = ptr->next) {
    curried = (*func)(ptr->data, curried);
  }

  return curried;
}

//--------------------------------------------------------------
// Empty?
//  Output:
//    true if the list is empty, otherwise, false.
//--------------------------------------------------------------
template <class T>
bool
List<T>::is_empty()
{
  if (head_ == NULL)  // assuming nobody messed with the list
    return true;
  return false;
}

//--------------------------------------------------------------
// Append
//  Append an item to the end of the list with a default
//  priority / key of 0.
//  Input:
//    T item      | the item to append
//--------------------------------------------------------------
template <class T>
void
List<T>::append(T item)
{
  Node<T>* element = new Node<T>();
  element->data = item;
  element->next = NULL;
  element->key = 0;
  size_++;
  
  if (this->is_empty()) {     // if it's empty
    head_ = element;       // the head ==
    tail_ = element;       // the tail
  } else {                    // else
    tail_->next = element; // append it to end and
    tail_ = element;       // make it the tail
  }
}

//--------------------------------------------------------------
// Prepend
//  Prepend an item to the beginning of the list with a default
//  priority / key of 0.
//  Input:
//    T item       | the item to prepend
//--------------------------------------------------------------
template <class T>
void
List<T>::prepend(T item)
{
  Node<T>* element = new Node<T>();
  element->data = item;
  element->next = NULL;
  element->key = 0;
  size_++;

  if (this->is_empty()) {     // the list is empty
    head_ = element;       // the head ==
    tail_ = element;       // the tail
  } else {                    // else
    element->next = head_; // prepend it to the head
    head_ = element;       // and make it the head
  }
}

//--------------------------------------------------------------
// Insert
//  Insert an element into the list in a manner to make the
//  list sorted (assuming that all other elements in the list
//  are already sorted).
//  Input:
//    T item        | the item to prepend
//    int key       | the priority of the item
//--------------------------------------------------------------
template <class T>
void
List<T>::insert(T item, int key)
{
  Node<T>* element = new Node<T>();
  Node<T>* iter;

  element->data = item;
  element->next = NULL;
  element->key = key;
  size_++;

  if (this->is_empty()) {          // if the list is empty
    head_ = element;                 // the head ==
    tail_ = element;                 // the tail
  } else if (key < head_->key) {   // else if the key < the head
    element->next = head_;           // prepend the element
    head_ = element;               // and make it the head
  } else {                           // otherwise
    iter = head_;
    for (iter; iter->next != NULL; iter = iter->next) {
      if (key < iter->next->key) {   // insert it at the proper
        element->next = iter->next;  // place in memory
        iter->next = element;
        return;
      }
    }
    tail_->next = element;          // can't find a match,
    tail_ = element;                // append the element.
  }
}

//--------------------------------------------------------------
// Poll
//  Remove an element from the beginning of the list.
//  Output:
//    The item's data or NULL if the list is empty.
//--------------------------------------------------------------
template <class T>
T
List<T>::poll()
{
  Node<T>* element = head_;
  T thing = NULL;

  if (!this->is_empty()) {
    size_--;
    thing = element->data;
    if (head_ == tail_) {          // There's one element in the list;
      head_ = NULL;                  // remove it and
      tail_ = NULL;                  // make the head and tail null.
    } else {                       // Otherwise,
      head_ = element->next;         // remove the head.
    }

    delete element;                // free up memory!
  }
  return thing;
}

//--------------------------------------------------------------
// Pop
//  Remove an element from the end of the list.
//  Output:
//    The item's data or NULL if the list is empty.
//--------------------------------------------------------------
template <class T>
T
List<T>::pop()
{
  Node<T>* iter = head_, *element;
  T thing;

  if (this->is_empty() ||           // empty list or
      head_ == tail_)               // a single element;
    return this->poll();              // poll the list
  else
    size_--;

  while (iter->next->next != NULL)  // iterate to the second to last
    iter = iter->next;              // element in the list

  thing = tail_->data;
  delete tail_;
  tail_ = iter;
  iter->next = NULL;

  return thing;
}

//--------------------------------------------------------------
// Remove
//  Remove the first element from the list that has a matching
//  key.
//  Inputs:
//    int key     | the key of the element to remove
//  Outputs:
//    The removed item's data or NULL if nothing matches.
//--------------------------------------------------------------
template <class T>
T
List<T>::remove(int key)
{
  Node<T>* iter = head_, *element;
  T thing = NULL;

  if (this->is_empty() ||           // empty list or
      (int)iter->key == key) {      // first element matches key
    return this->poll();              // use poll to remove
  }
  
  while (iter->next->next != NULL) {// Remove an item in the
    if (iter->next->key == key) {   // middle of the list
      element = iter->next;
      iter->next = iter->next->next;
      thing = element->data;
      delete element;
      size_--;
      break;
    }
      
    iter = iter->next;
  }

  if (thing == NULL && 
      iter->next->key == key) {     // Remove the last element
    element = iter->next;             // of the list
    iter->next = NULL;
    tail_ = iter;
    thing = element->data;
    delete element;
    size_--;
  }

  return thing;
}

//--------------------------------------------------------------
// Iterator
//  Returns an iterator over the list.
//--------------------------------------------------------------
template <class T>
Iterator<T>
List<T>::iterator()
{
  return *(new Iterator<T>(head_));
}

#endif // ARDUINO_LINKED_LIST
