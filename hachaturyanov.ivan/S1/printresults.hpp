#ifndef PRINTRESULTS_HPP
#define PRINTRESULTS_HPP

#include <iostream>
#include "../common/list.hpp"

namespace hachaturyanov {
  using Pair = std::pair< std::string, hachaturyanov::List< size_t > >;

  void printNames(std::ostream &out, const List< Pair > &pairs);
  void printNumbers(std::ostream &out, const List< Pair > &pairs);
  void printSums(std::ostream &out, const List< Pair > &pairs);
}

#endif
