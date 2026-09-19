#include "commands.hpp"

namespace loseva {

void cmdPrint(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string name;
  if (!(is >> name)) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * pTree = dms.find({name, Dataset{}});
  if (!pTree) {
    throw CommandException("<INVALID COMMAND>");
  }
  const Dataset & ds = pTree->tree;
  if (ds.empty()) {
    os << "<EMPTY>\n";
  } else {
    bool first = true;
    for (auto it = ds.cbegin(); it != ds.cend(); ++it) {
      if (!first) {
        os << " ";
      }
      os << *it;
      first = false;
    }
    os << "\n";
  }
}

}
