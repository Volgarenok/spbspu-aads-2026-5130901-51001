#ifndef YAFU_PARSER_HPP
#define YAFU_PARSER_HPP

#include "command.hpp"
#include "dynarray.hpp"
#include "string.hpp"

namespace yafu {
  bool parseArguments(int argc, char** argv, Command& result);
  bool parseInteractiveLine(const String& line, Command& result, bool& isExit);
  void splitIntoTokens(const String& line, DynArray< String >& result);
}

#endif
