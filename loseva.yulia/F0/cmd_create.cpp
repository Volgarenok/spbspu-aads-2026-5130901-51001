#include "commands.hpp"

namespace loseva {

void cmdCreate(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name) || dms.has({name, Dataset{}})) {
    throw CommandException("<INVALID COMMAND>");
  }
  dms.insert({name, Dataset{}});
  os << "Создано дерево: " << name << "\n";
}

}
