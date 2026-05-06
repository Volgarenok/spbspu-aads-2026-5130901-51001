#include "commands.hpp"

namespace alekseev
{
  void printInvalid(std::ostream& out)
  {
    out << InvalidCommand << '\n';
  }

  CommandTable makeCommandTable()
  {
    return CommandTable();
  }

  void dispatchCommand(const CommandTable&, const std::string&, DictionaryStorage&,
      std::ostream& out)
  {
    printInvalid(out);
  }
}
