/*******************************************************************************
  File:   heap.h                                                               *
  Author: Daniel Pesu, dp604@uowmail.edu.au, 4726686                           *
  Ass.:   CSCI203, Assignment 2                                                *
  About:  This class forms a templated heap datastructure which is based on    *
          the Circular Queue object.                                           *
                                                                               *
  Last Modified: 04/Sep/16.                                                    *
*******************************************************************************/

#ifndef HEAP_H_
#define HEAP_H_
#include "../circularbuffer/circularbuffer.h"

namespace datastructures
{
  template <class T>
  class Heap
  {
   public:
    void Insert(T data);
    T Delete(typename CircularBuffer<T>::Iterator node);

    bool isEmpty() const;

    typename CircularBuffer<T>::Iterator Top() { return heap_.start(); }

    void SiftUp(typename CircularBuffer<T>::Iterator node);
    void SiftDown(typename CircularBuffer<T>::Iterator node);

   private:
    CircularBuffer<T> heap_;
    void swap(typename CircularBuffer<T>::Iterator a, typename CircularBuffer<T>::Iterator b);
    typename CircularBuffer<T>::Iterator smallest_child(typename CircularBuffer<T>::Iterator node);
  };


  template <class T>
  void Heap<T>::Insert(T data)
  {
    heap_.push_back(data);
    SiftUp(heap_.end());
  }

  template <class T>
  T Heap<T>::Delete(typename CircularBuffer<T>::Iterator node)
  {
    swap(node, heap_.end());

    T data = heap_.pop_back();
    SiftDown(node);

    return data;
  }

  /*****************************************************************************
    is Empty                                             Time Complexity: O(1) *
    Returns true if the heap is empty.
  *****************************************************************************/
  template <class T>
  bool Heap<T>::isEmpty() const
  {
    if (heap_.length() == 0)
      return true;
    else
      return false;
  }

  /*****************************************************************************
    Sift Up                                                                    *
    Performs a sift up operation on a given node.                              *
  *****************************************************************************/
  template <class T>
  void Heap<T>::SiftUp(typename CircularBuffer<T>::Iterator node)
  {
    if (node.position() != 0)  // node is not root.
    {
      typename CircularBuffer<T>::Iterator parent(heap_, node.position()/2);
      if (*node < *parent)
      {
        swap(node, parent);
        SiftUp(parent);
      }
    }
  }

  /*****************************************************************************
    Sift Down                                                                  *
    Performs a sift down operation on a given node.                            *
  *****************************************************************************/
  template <class T>
  void Heap<T>::SiftDown(typename CircularBuffer<T>::Iterator node)
  {
    if ((node.position() + 1) * 2 - 1 < heap_.length())  // Not a leaf node.
    {
      typename CircularBuffer<T>::Iterator smallest = smallest_child(node);
      if (*smallest < *node)
      {
        swap(node, smallest);
        SiftDown(smallest);
      }
    }
  }

  /*****************************************************************************
    swap
    Swaps the values pointed to by two iterators in the Heap.
  *****************************************************************************/
  template <class T>
  void Heap<T>::swap(typename CircularBuffer<T>::Iterator a, typename CircularBuffer<T>::Iterator b)
  {
    T temp = *a;
    heap_[a.position()] = *b;
    heap_[b.position()] = temp;
  }

  /*****************************************************************************
    smallest_child
    Returns an iterator pointing to the smallest child of a given node.
    Assumes that the node has at least one child.
  *****************************************************************************/
  template <class T>
  typename CircularBuffer<T>::Iterator Heap<T>::smallest_child(typename CircularBuffer<T>::Iterator node)
  {
    int left_child_index = (node.position() + 1) * 2 - 1;
    typename CircularBuffer<T>::Iterator left_child(heap_, left_child_index);

    if (left_child_index + 1 < heap_.length())  // Has a right child.
    {
      typename CircularBuffer<T>::Iterator right_child(heap_, left_child_index + 1);
      if (*left_child > *right_child)
        return right_child;
    }

    return left_child;
   }
}
#endif  // HEAP_H_
