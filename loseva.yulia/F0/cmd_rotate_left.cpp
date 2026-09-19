#include "commands.hpp"

namespace loseva {

void cmdRotateLeft(std::istream & is, std::ostream & os, DatasetsMap & dms)
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
  try {
    pTree->tree.rotateLeftManual(val);
    os << "Выполнен левый поворот для узла " << val << " в дереве " << name << "\n";
  } catch (...) {
    throw CommandException("<INVALID COMMAND>");
  }
}

}
