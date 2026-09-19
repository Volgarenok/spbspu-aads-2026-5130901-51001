#include "commands.hpp"

namespace loseva {

void cmdSearch(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  int val = 0;
  if (!(is >> name >> val)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({name, Dataset{}});
  if (!pTree) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << (pTree->tree.has(val) ? "Элемент найден\n" : "Элемент не найден\n");
}

}
