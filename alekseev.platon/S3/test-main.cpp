#include <iostream>

int runContainerHashTests();
int runGraphCommandTests();

int main()
{
  const int containerResult = runContainerHashTests();
  if (containerResult != 0)
  {
    return containerResult;
  }

  const int graphResult = runGraphCommandTests();
  if (graphResult != 0)
  {
    return graphResult + 1;
  }

  return 0;
}
