#include "sequence.hpp"

#include <iostream>

int main()
{
  using namespace yarmolinskaya;
  List< NamedSequence > data;
  readSequences(data);
  if (data.empty())
  {
    std::cout << 0 << "\n";
    return 0;
  }
  printNames(data);
  processSequences(data);
  return 0;
}
