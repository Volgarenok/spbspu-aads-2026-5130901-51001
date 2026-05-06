#include <iostream>

int runBSTreeTests();
int runParserTests();
int runDatasetTests();
int runCommandTests();

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
  const int datasetResult = runDatasetTests();
  if (datasetResult != 0)
  {
    return datasetResult + 2;
  }
  const int commandResult = runCommandTests();
  if (commandResult != 0)
  {
    return commandResult + 3;
  }
  return 0;
}
