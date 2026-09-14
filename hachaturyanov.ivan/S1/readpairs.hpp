#ifndef READPAIRS_HPP
#define READPAIRS_HPP

#include <iostream>
#include "../common/list.hpp"

namespace hachaturyanov {
  using Pair = std::pair< std::string, hachaturyanov::List< size_t > >;

  void skipSpaces(std::istream &in);
  bool isLineEnded(std::istream &in);
  void nextLine(std::istream &in);

  List< Pair > readPairs(std::istream &in);
}

#endif
