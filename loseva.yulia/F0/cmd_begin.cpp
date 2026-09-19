#include "commands.hpp"

namespace loseva {

void cmdBegin(std::istream & is, std::ostream & os, DatasetsMap & dms, IteratorsMap & ims)
{
  std::string iterName, treeName;
  if (!(is >> iterName >> treeName)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({treeName, Dataset{}});
  if (!pTree) {
    throw CommandException("<INVALID COMMAND>");
  }
  IteratorPair * pIter = ims.find({iterName, Dataset::const_iterator{}});
  if (pIter) {
    pIter->iter = pTree->tree.cbegin();
  } else {
    ims.insert({iterName, pTree->tree.cbegin()});
  }
  os << "Итератор " << iterName << " привязан к дереву " << treeName
     << " и установлен на его начало\n";
}

}
