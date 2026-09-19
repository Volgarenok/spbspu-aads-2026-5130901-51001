#include "commands.hpp"

namespace loseva {

void cmdNext(std::istream & is, std::ostream & os, IteratorsMap & ims)
{
  std::string iterName;
  if (!(is >> iterName)) {
    throw CommandException("<INVALID COMMAND>");
  }
  IteratorPair * pIter = ims.find({iterName, Dataset::const_iterator{}});
  if (!pIter || pIter->iter.getNode() == nullptr) {
    throw CommandException("<INVALID COMMAND>");
  }
  ++(pIter->iter);
  os << "Итератор " << iterName << " перешел к следующему элементу\n";
}

}
