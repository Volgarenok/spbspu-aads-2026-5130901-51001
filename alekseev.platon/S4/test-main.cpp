#include <iostream>

int runBSTreeTests();
int runParserTests();

int main()
{
  const int bstResult = runBSTreeTests();
  if (bstResult != 0)
  {
    return bstResult;
  }
  const int parserResult = runParserTests();
  if (parserResult != 0)
  {
    return parserResult + 1;
  }
  return 0;
}
