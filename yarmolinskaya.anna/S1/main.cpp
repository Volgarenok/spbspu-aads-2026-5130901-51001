#include "sequence.hpp"

#include <iostream>

int main()
{
  using namespace yarmolinskaya;
  List< NamedSequence > data;
  bool is_overflow = false;

  readSequences(data, is_overflow);

  if (data.empty())
  {
    std::cout << 0 << "\n";
    if (is_overflow)
    {
      std::cerr << "overflow\n";
      return 1;
    }
    return 0;
  }

  printNames(data);
  processSequences(data);

  if (is_overflow)
  {
    std::cerr << "overflow\n";
    return 1;
  }

  return 0;
}
