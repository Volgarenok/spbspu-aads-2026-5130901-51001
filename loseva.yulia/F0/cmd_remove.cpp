#include "commands.hpp"

namespace loseva {

void cmdRemove(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({name, Dataset{}});
  if (!pTree || !pTree->tree.remove(val)) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Из дерева " << name << " удалён элемент: " << val << "\n";
}

}
