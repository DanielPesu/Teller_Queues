/*******************************************************************************
  File: circularbuffer.h                                                       *
  Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                           *
  Ass.:   CSCI203, Assignment 2                                                *
  About:  This file holds the definitions for a circular buffer class which is *
          an automatic resizing array.                                         *
          for efficiency, a logical start and end (determined by start_ and    *
          end_) are used to make push_front() operations O(1) time complexity  *
                                                                               *
  Last Modified: 30/Aug/16.                                                    *
*******************************************************************************/
#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_
  
#include <cstddef>  // NULL
#include <stdexcept>
#include <iostream>
namespace datastructures
{
  /*****************************************************************************
    Circular Buffer.                                                           *
    This class stores data in a circular buffer.                               *
    The logical start refers to the position of the 'start' pointer and the    *
    physical start refers to the first element of the buffer array. Likewise   *
    for end.                                                                   *
  *****************************************************************************/
  template <class T>
  class CircularBuffer
  {
   public:
    class Iterator;
    friend class Iterator;

    CircularBuffer();
    ~CircularBuffer();

    int  length() const;
    bool isFull() const;

    Iterator start();
    Iterator end();

    void push_front(T data);
    void push_back(T data);

    T pop_front();
    T pop_back();

    T& operator[](int index);

   private:
    T*  buffer_;   // Stores values in a resizeable array.
    int start_;   // The index of the logical start of the buffer.
    int end_;     // The index of the logical end of the buffer.
    int length_;  // The length of a run in the buffer.
    int size_;    // Holds the current size of the array.
  
    void slide(int& pos, int dist);  // Physical elements.
    void resize(int size);
    void empty();
  };

  /*****************************************************************************
    Iterator                                                                   *
    This class allows controled navigation through the Circular Buffer.        *
    The iterator wraps back to the start once it passes the physical end of    *
    the buffer.                                                                *
    An iterator must be linked to a buffer during instantiation and cannot be  *
    unlinked throughout its lifetime.                                          *
  *****************************************************************************/
  template <class T>
  class CircularBuffer<T>::Iterator
  {
   public:
    Iterator(CircularBuffer<T>& buffer, int pos = 0) : buffer_ref_(buffer), pos_(pos) {}

    int position() const { return pos_; }

    void replace(T data);

    bool isEmpty() const;

    T         operator *  ();
    bool      operator == (const Iterator& source);
    bool      operator != (const Iterator& source);
    Iterator& operator =  (const Iterator& source);
    Iterator  operator ++ (int);
    Iterator  operator -- (int);
    Iterator& operator ++ ();
    Iterator& operator -- ();
    Iterator& operator +  (int dist);
    Iterator& operator -  (int dist);

   private:
    CircularBuffer<T>& buffer_ref_;  // Reference to the associated Buffer.
    int pos_;  // iterator's position from the logical start.
  };

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~ Circular Buffer ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*****************************************************************************
    Constructor                                                                *
  *****************************************************************************/
  template <class T>
  CircularBuffer<T>::CircularBuffer()
  {
    buffer_ = NULL;
    start_ = end_ = size_ = length_ = 0;
  }

  /*****************************************************************************
    Destructor                                                                 *
  *****************************************************************************/
  template <class T>
  CircularBuffer<T>::~CircularBuffer()
  {
    if (buffer_ != NULL)
    {
      delete [] buffer_;
      buffer_ = NULL;
    }
    start_ = end_ = size_ = length_ = 0;
  }

  /*****************************************************************************
    length                                               Time Complexity: O(1) *
    Returns the logical size of the buffer.                                    *
  *****************************************************************************/
  template <class T>
  int CircularBuffer<T>::length() const
  {
    return length_;
  }

  /*****************************************************************************
    isFull
  *****************************************************************************/
  template <class T>
  bool CircularBuffer<T>::isFull() const
  {
    return length_ == size_;
  }

  /*****************************************************************************
    start                                                                      *
    Returns an iterator object which points to the logical start of the        *
    buffer.                                                                    *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator CircularBuffer<T>::start()
  {
    return typename CircularBuffer<T>::Iterator(*this, 0);
  }

  /*****************************************************************************
    end                                                                        *
    Returns an iterator object which points to the logical end of the buffer.  *
  *****************************************************************************/
  template <class T>
  typename  CircularBuffer<T>::Iterator CircularBuffer<T>::end()
  {
    return typename CircularBuffer<T>::Iterator(*this, length_-1);
  }

  /*****************************************************************************
    push_front             Time Complexity: Best-case: O(1) | Worst-case: O(n) *
    Inserts a given item to the logical front of the buffer. If the buffer is  *
    full, then it is resized to twice its current length.                      *
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::push_front(T data)
  {
    if (size_ == 0)
      resize(1);
    else if (isFull())
      resize(size_ * 2);

    slide(start_, -1);
    buffer_[start_] = data;
    ++length_;
  }

  /*****************************************************************************
    push_back              Time Complexity: Best-case: O(1) | Worst-case: O(n) *
    Inserts a given item to the logical back of the buffer. If the buffer is   *
    full, then it is resized to twice its current length.                      *
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::push_back(T data)
  {
    if (size_ == 0)
      resize(1);
    else if (isFull())
      resize(size_ * 2);

    slide(end_, +1);

    buffer_[end_] = data;
    ++length_;
  }

  /*****************************************************************************
    pop_front                                            Time Complexity: O(1) *
    Removes the item at the logical front of the buffer.                       *
    If the buffer is empty then an exception is thrown.                        *
  *****************************************************************************/
  template <class T>
  T CircularBuffer<T>::pop_front()
  {
    if (size_ == 0)
      throw std::underflow_error("Attempted to pop empty buffer");

    T pop = buffer_[start_];

    if (start_ == end_)
      empty();
    else
      slide(start_, +1);

    --length_;
    return pop;
  }

  /*****************************************************************************
    pop_back                                             Time Complexity: O(1) *
    Removes the item at the logical end of the buffer.                         *
    If the buffer is empty then an exception is thrown.                        *
  *****************************************************************************/
  template <class T>
  T CircularBuffer<T>::pop_back()
  {
    if (size_ == 0)
      throw std::underflow_error("Attempted to pop empty buffer");

    T pop = buffer_[end_];

    if (start_ == end_)
      empty();
    else
      slide(end_, -1);

    --length_;
    return pop;
  }

  /*****************************************************************************
    Subscript operator
  *****************************************************************************/
  template <class T>
  T& CircularBuffer<T>::operator[](int index)
  {
    return buffer_[(start_ + (index % length_)) % size_];
  }

  /*****************************************************************************
    slide                                                Time Complexity: O(1) *
    Moves a pointer a given distance along the buffer, wrapping back to the    *
    front once it has passed the physical end of the buffer.                   *
    Assumes that the passed pointer is in-fact pointing to an element of this  *
    buffer.                                                                    *
    The special case of a buffer of size 1 is delt with by not moving the      *
    pointer.                                                                   *
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::slide(int& index, int dist)
  {
    index = (index + dist) % size_;
    if (index < 0)
      index += size_;
  }

  /*****************************************************************************
    resize                                               Time Complexity: O(n) *
    Expands or contracts the buffer, shifting the logical start to the         *
    beginning of the array. Doing so simplifies management of iterators.       *
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::resize(int size)
  {
    T* temp = buffer_;
    int i = start_;

    buffer_ = new T[size];
    for (int j = 0; j < length_; ++j)
    {
      buffer_[j] = temp[i++ % size_];
    }

    if (temp != NULL)
	    delete [] temp;

    start_ = 0;
    if (length_ == 0)
      end_ = 0;
    else
      end_ = length_ - 1;

    size_ = size;
  }

  /*****************************************************************************
    empty                                                Time Complexity: O(1) *
    Empties the contents of the buffer, freeing all dynamic memory.            *
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::empty()
  {
    delete [] buffer_;
    buffer_ = NULL;
    end_ = start_ = size_ = 0;
  }

  /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Iterator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
  /*****************************************************************************
    replace
    Replaces the data at the current position with the passed data.
  *****************************************************************************/
  template <class T>
  void CircularBuffer<T>::Iterator::replace(T data)
  {
    buffer_ref_[pos_] = data;
  }

  /*****************************************************************************
    Dereference operator                                 Time Complexity: O(1) *
    Returns, by value, a copy of the data that the iterator is currently       *
    pointing to.                                                               *
  *****************************************************************************/
  template <class T>
  T CircularBuffer<T>::Iterator::operator*()
  {
    return buffer_ref_[pos_];
  }

  /*****************************************************************************
    Increment operator                                   Time Complexity: O(1) *
    Moves the iterator forwards along the buffer. If the logical size is zero, *
    then the iterator is not moved.                                            *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator& CircularBuffer<T>::Iterator::operator+(int distance)
  {
    pos_ = (pos_ + distance) % buffer_ref_.length_;
    if (pos_ < 0)
      pos_ = pos_ + buffer_ref_.size_;
    return *this;
  }

  /*****************************************************************************
    Decrement operator                                   Time Complexity: O(1) *
    Moves the iterator backwards along the buffer. If the logical size is      *
    zero, then the iterator is not moved.                                      *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator& CircularBuffer<T>::Iterator::operator-(int distance)
  {
    return *(this->operator+(distance));
  }

  /*****************************************************************************
    Assignment operator                                                        *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator& CircularBuffer<T>::Iterator::operator=(const CircularBuffer<T>::Iterator& source)
  {
    buffer_ref_ = source.buffer_ref_;
    pos_ = source.pos_;
    return *this;
  }

  /*****************************************************************************
    Equality operator                                    Time Complexity: O(1) *
    Returns true if two iterators are pointing to the same element in the same *
    buffer.                                                                    *
  *****************************************************************************/
  template <class T>
  bool CircularBuffer<T>::Iterator::operator==(const CircularBuffer<T>::Iterator& source)
  {
    return (buffer_ref_.buffer_ == source.buffer_ref_.buffer_) && (pos_ == source.pos_);
  }

  /*****************************************************************************
    Inequality operator                                                        *
  *****************************************************************************/
  template <class T>
  bool CircularBuffer<T>::Iterator::operator!=(const CircularBuffer<T>::Iterator& source)
  {
    return !(*this == source);
  }

  /*****************************************************************************
    Post-increment operator                                                    *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator CircularBuffer<T>::Iterator::operator++(int)
  {
    CircularBuffer<T>::Iterator hold(buffer_ref_, pos_);
    *this + 1;

    return hold;
  }

  /*****************************************************************************
    Post-decrement operator                                                    *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator CircularBuffer<T>::Iterator::operator--(int)
  {
    CircularBuffer<T>::Iterator hold(buffer_ref_, pos_);
    *this - 1;

    return hold;
  }

  /*****************************************************************************
    Pre-increment operator                                                     *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator& CircularBuffer<T>::Iterator::operator++()
  {
    return *this + 1;
  }

  /*****************************************************************************
    Pre-decrement operator                                                     *
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator& CircularBuffer<T>::Iterator::operator--()
  {
    return *this - 1;
  }
}
#endif  // _CIRCULARBUFFER_H_
