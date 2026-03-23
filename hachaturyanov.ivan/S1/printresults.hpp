#ifndef PRINTRESULTS_HPP
#define PRINTRESULTS_HPP

#include <iostream>
#include "list.hpp"

namespace hachaturyanov {
  using Pair = std::pair< std::string, hachaturyanov::List< int > >;

  void printNames(std::ostream &out, const List< Pair > &pairs);
}

#endif
