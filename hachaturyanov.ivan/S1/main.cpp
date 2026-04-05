#include <iostream>
#include "readpairs.hpp"
#include "printresults.hpp"

int main()
{

  hachaturyanov::List< hachaturyanov::Pair > pairs = hachaturyanov::readPairs(std::cin);

  if (pairs.isEmpty()) {
    std::cout << "0" << "\n";
    return 0;
  }

  hachaturyanov::printNames(std::cout, pairs);
  hachaturyanov::printNumbers(std::cout, pairs);

  try {
    hachaturyanov::printSums(std::cout, pairs);
  } catch (const std::overflow_error &e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
  return 0;
}
