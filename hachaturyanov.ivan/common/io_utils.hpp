#ifndef IO_UTILS_HPP
#define IO_UTILS_HPP

#include <iostream>
#include "list.hpp"

namespace hachaturyanov
{
  List< std::string > split(std::string line);
  bool isdigit(std::string str);
}

#endif
