#include "commands.hpp"

namespace loseva {

void cmdClear(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({name, Dataset{}});
  if (!pTree) {
    throw CommandException("<INVALID COMMAND>");
  }
  pTree->tree.clear();
  os << "Дерево " << name << " очищено\n";
}

}
