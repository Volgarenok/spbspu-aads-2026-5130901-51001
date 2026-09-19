#include "commands.hpp"

namespace loseva {

void cmdRotateRight(std::istream & is, std::ostream & os, DatasetsMap & dms)
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
    pTree->tree.rotateRightManual(val);
    os << "Выполнен правый поворот для узла " << val << " в дереве " << name << "\n";
  } catch (...) {
    throw CommandException("<INVALID COMMAND>");
  }
}

}
