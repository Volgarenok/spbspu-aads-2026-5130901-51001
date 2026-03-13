#ifndef INPUT_HPP
#define INPUT_HPP

#include "list.hpp"
#include <iostream>
#include <cstring>

namespace vishnyakov
{
  struct Sequence
  {
    std::string name;
    List< size_t >* nums;
  };

  List< Sequence > readInput(std::istream& in);
  void getWitoutSkips(std::istream& in);
  bool isEnd(std::istream& in);
  void skipLine(std::istream& in);
}

#endif