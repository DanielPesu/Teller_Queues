#include <iostream>
#include "circularbuffer.h"
using namespace std;
using namespace datastructures;

int main() {
  cout << "Constructing Buffers.." << endl;
  CircularBuffer<int> buffer_1;
  CircularBuffer<int> buffer_2;

  cout << "Constructing iterators.." << endl;
  CircularBuffer<int>::Iterator iter_1(buffer_1);
  CircularBuffer<int>::Iterator iter_2(buffer_2);

  int k = 5;
  cout << "Testing push.." << endl;
  cout << "   : Pushing to front data 1.." << k << ", pushing to back data 11.." << k+10 << endl;
  for (int i = 1; i <= k; ++i)
  {
    buffer_1.push_front(i);
    buffer_1.push_back(i+10);
  }

  iter_1 = buffer_1.start();
  bool flag = true;
  for (int i = k; i > 0; --i)
  {
    if (*iter_1++ != i)
      flag = false;
  }
  for (int i = 1; i <= k; ++i)
  {
    if (*iter_1++ != i + 10)
      flag = false;
  }
  if (flag)
    cout << "Push Successful." << endl;
  else
    cerr << "Push Failed." << endl;

  cout << "Testing pop.." << endl;
  flag = true;
  for (int i = k; i > 0; --i)
  {
    if (buffer_1.pop_front() != i)
    {
      cout << "pop_front with i=" << i << endl;
      flag = false;
    }
    if (buffer_1.pop_back() != i + 10)
    {
      cout << "pop_back with i=" << i << endl;
      flag = false;
    }
  }
  if (flag)
    cout << "Pop Successful." << endl;
  else
    cerr << "Pop Failed." << endl;

  cout << "Popping empty buffer.." << endl;
  try {
    buffer_1.pop_front();
  }
  catch (std::underflow_error e) {
    cout << "Exception Successfully Thrown with error \'" << e.what() << "\'." << endl;
    cout << "Attempting to use buffer.." << endl;
    cout << "   : Testing push..";
    buffer_1.push_front(13);
    buffer_1.push_back(42);
    iter_1 = buffer_1.start();
    if (*iter_1 == 13 && *(iter_1 + 1) == 42)
      cout << "PASS" << endl;
    else
      cout << "FAIL" << endl;

    cout << "   : Tesiting pop..";
    if (buffer_1.pop_back() == 42 && buffer_1.pop_front() == 13)
      cout << "PASS" << endl;
    else
      cout << "FAIL" << endl;    
  }
  cout << "Testing Complete." << endl;
  return 0;
}
