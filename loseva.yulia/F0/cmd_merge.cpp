#include "commands.hpp"

namespace loseva {

void cmdMerge(std::istream & is, std::ostream & os, DatasetsMap & dms)
{
  std::string target, src1, src2;
  if (!(is >> target >> src1 >> src2)) {
    throw CommandException("<INVALID COMMAND>");
  }
  if (dms.has({target, Dataset{}})) {
    throw CommandException("<INVALID COMMAND>");
  }
  DatasetPair * p1 = dms.find({src1, Dataset{}});
  DatasetPair * p2 = dms.find({src2, Dataset{}});
  if (!p1 || !p2) {
    throw CommandException("<INVALID COMMAND>");
  }
  Dataset merged;
  for (auto it = p1->tree.cbegin(); it != p1->tree.cend(); ++it) {
    merged.insert(*it);
  }
  for (auto it = p2->tree.cbegin(); it != p2->tree.cend(); ++it) {
    merged.insert(*it);
  }
  dms.insert({target, std::move(merged)});
  os << "Создано объединённое дерево: " << target << "\n";
}

}
