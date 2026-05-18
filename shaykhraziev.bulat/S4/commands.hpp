#ifndef S4_COMMANDS_HPP
#define S4_COMMANDS_HPP

#include "io.hpp"

#include <iosfwd>
#include <string>

namespace shaykhraziev
{
  bool executeCommandLine(DatasetTable& datasets, const std::string& line, std::ostream& out);
  void processCommands(DatasetTable& datasets, std::istream& in, std::ostream& out);
}

#endif
