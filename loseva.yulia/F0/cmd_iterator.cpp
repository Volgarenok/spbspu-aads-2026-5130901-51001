#include "commands.hpp"

namespace loseva {

void cmdIterator(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName)) {
    throw CommandException("<INVALID COMMAND>");
  }
  IteratorPair * pIter = ims.find({iterName, Dataset::const_iterator{}});
  if (!pIter || pIter->iter.getNode() == nullptr) {
    throw CommandException("<INVALID COMMAND>");
  }
  os << "Текущий элемент итератора " << iterName << ": " << *(pIter->iter) << "\n";
}

}
