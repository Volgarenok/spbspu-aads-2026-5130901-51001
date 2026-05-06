#include <iostream>

int runBSTreeTests();

int main()
{
  const int bstResult = runBSTreeTests();
  if (bstResult != 0)
  {
    return bstResult;
  }
  return 0;
}
