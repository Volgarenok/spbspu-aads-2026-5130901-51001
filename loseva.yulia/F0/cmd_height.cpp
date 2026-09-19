#include "commands.hpp"

namespace loseva {

void cmdHeight(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({name, Dataset{}});
  if (!pTree) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Высота дерева " << name << ": " << pTree->tree.height() << "\n";
}

}
