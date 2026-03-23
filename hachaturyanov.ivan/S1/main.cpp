#include <iostream>
#include "readpairs.hpp"

int main()
{
  hachaturyanov::List< hachaturyanov::Pair > pairs = hachaturyanov::readPairs(std::cin);

  if (pairs.isEmpty()) {
    std::cout << "0" << "\n";
    return 0;
  }


}
