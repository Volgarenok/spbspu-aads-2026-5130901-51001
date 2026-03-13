#ifndef INPUT_HPP
#define INPUT_HPP

#include "list.hpp"
#include <iostream>
#include <cstring>

namespace vishnyakov
{
  using Sequence = std::pair<std::string, List< size_t >>;

  List< Sequence > readInput(std::istream& in);
  void getWitoutSkips(std::istream& in);
  bool isEnd(std::istream& in);
  void skipLine(std::istream& in);
}

#endif