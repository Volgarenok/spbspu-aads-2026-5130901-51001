#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>
#include <istream>
#include <ostream>

#include "storage.hpp"
#include "bank.hpp"

namespace smirnova
{
  void addClientCommand(CommandContext&, std::istream&, std::ostream&);
  void removeClientCommand(CommandContext&, std::istream&, std::ostream&);

  void listCommand(CommandContext&, std::istream&, std::ostream&);
  void reportCommand(CommandContext&, std::istream&, std::ostream&);
  void approvedCommand(CommandContext&, std::istream&, std::ostream&);
  void showCommand(CommandContext& ctx, std::istream& in, std::ostream& out);

  void rejectedCommand(CommandContext&, std::istream&, std::ostream&);
  void saveCommand(CommandContext&, std::istream&, std::ostream&);

  void printHelp(std::ostream&);
  void executeCommands(std::istream&, std::ostream&);
}

#endif

