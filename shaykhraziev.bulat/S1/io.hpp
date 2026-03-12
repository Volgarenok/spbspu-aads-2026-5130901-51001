#ifndef IO_HPP
#define IO_HPP

#include <iostream>

namespace shaykhraziev
{
  void skipSpaces(std::istream& in);
  bool isLineEnd(std::istream& in);
  void skipLine(std::istream& in);
}

#endif

