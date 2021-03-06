/*******************************************************************************
  File:   queue.h                                                              *
  Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                           *
  Ass.:   CSCI203, Assignment 2                                                *
  About:  This file holds the definitions for a Queue class which is based on  *
          the Circular Buffer class.                                           *
                                                                               *
  Last Modified: 04/Sep/16.                                                    *
*******************************************************************************/
#ifndef QUEUE_H_
#define QUEUE_H_

#include "../circularbuffer/circularbuffer.h"
namespace datastructures
{
  template <class T>
  class Queue;

  template <typename T>
  bool operator<(const Queue<T>& lhs, const Queue<T>& rhs);

  template <typename T>
  bool operator>(const Queue<T>& lhs, const Queue<T>& rhs);

  template <class T>
  class Queue
  {
   public:
    bool isEmpty() { return !(queue_.length()); }

    int Length() { return queue_.length(); }

    void Enqueue(T data) { queue_.push_back(data); }
    T Dequeue() { return queue_.pop_front(); }

    friend bool operator< <>(const Queue<T>& lhs, const Queue<T>& rhs);
    friend bool operator> <>(const Queue<T>& lhs, const Queue<T>& rhs);

   private:
    CircularBuffer<T> queue_;
  };

  template <typename T>
  bool operator<(const Queue<T>& lhs, const Queue<T>& rhs)
  { 
    return (lhs.queue_.length() < rhs.queue_.length());
  }


  template <typename T>
  bool operator>(const Queue<T>& lhs, const Queue<T>& rhs)
  {
    return (lhs.queue_.length() > rhs.queue_.length());
  }
}

#endif  // QUEUE_H_ 
