#include "heap.h"
using namespace std;
using namespace datastructures;

int main()
{
  Heap<int> myHeap;

  for (int i = 19; i > 0; --i)
  {
    myHeap.Insert(i);
    myHeap.SHOWME();
  }

  for (int i = 0; i < 19; ++i)
  {
    int data = myHeap.Delete(myHeap.Top());
    cout << "Removed \'" << data << "\'" << endl;
  }
  return 0;
}
