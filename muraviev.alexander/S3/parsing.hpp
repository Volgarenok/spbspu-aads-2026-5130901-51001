#ifndef PARSING_HPP
#define PARSING_HPP

#include <string>
#include <vector>

namespace muraviev
{
  bool splitStrictSpaces(const std::string& line, std::vector< std::string >& tokens);
}

#endif
