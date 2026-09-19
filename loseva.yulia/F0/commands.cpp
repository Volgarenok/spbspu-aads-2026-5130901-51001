#include "commands.hpp"
#include <functional>
#include <limits>

namespace loseva {

using Handler = std::function< void(
  std::istream &,
  std::ostream &,
  DatasetsMap &,
  IteratorsMap &) >;

struct CommandItem {
  std::string name;
  Handler handler;

  bool operator<(const CommandItem & rhs) const noexcept
  {
    return name < rhs.name;
  }
  bool operator>(const CommandItem & rhs) const noexcept
  {
    return name > rhs.name;
  }
  bool operator==(const CommandItem & rhs) const noexcept
  {
    return name == rhs.name;
  }
};

namespace {

void runBegin(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap & ims)
{
  cmdBegin(is, os, dms, ims);
}

void runClear(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdClear(is, os, dms);
}

void runContains(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdContains(is, os, dms);
}

void runCreate(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdCreate(is, os, dms);
}

void runHeight(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdHeight(is, os, dms);
}

void runInsert(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdInsert(is, os, dms);
}

void runIterator(
  std::istream & is,
  std::ostream & os,
  DatasetsMap &,
  IteratorsMap & ims)
{
  cmdIterator(is, os, ims);
}

void runMerge(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdMerge(is, os, dms);
}

void runNext(
  std::istream & is,
  std::ostream & os,
  DatasetsMap &,
  IteratorsMap & ims)
{
  cmdNext(is, os, ims);
}

void runPrint(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdPrint(is, os, dms);
}

void runRemove(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdRemove(is, os, dms);
}

void runRotateLeft(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdRotateLeft(is, os, dms);
}

void runRotateRight(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdRotateRight(is, os, dms);
}

void runSearch(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdSearch(is, os, dms);
}

void runTree(
  std::istream & is,
  std::ostream & os,
  DatasetsMap & dms,
  IteratorsMap &)
{
  cmdTree(is, os, dms);
}

}

void processCommands(std::istream & in, std::ostream & os)
{
  DatasetsMap datasets;
  IteratorsMap iterators;

  AVLTree< CommandItem > commandTree;

  commandTree.insert({"begin", runBegin});
  commandTree.insert({"clear", runClear});
  commandTree.insert({"contains", runContains});
  commandTree.insert({"create", runCreate});
  commandTree.insert({"height", runHeight});
  commandTree.insert({"insert", runInsert});
  commandTree.insert({"iterator", runIterator});
  commandTree.insert({"merge", runMerge});
  commandTree.insert({"next", runNext});
  commandTree.insert({"print", runPrint});
  commandTree.insert({"remove", runRemove});
  commandTree.insert({"rotate-left", runRotateLeft});
  commandTree.insert({"rotate-right", runRotateRight});
  commandTree.insert({"search", runSearch});
  commandTree.insert({"tree", runTree});

  std::string cmd;
  while (in >> cmd) {
    try {
      CommandItem dummy{cmd, nullptr};
      CommandItem * item = commandTree.find(dummy);
      if (!item) {
        throw CommandException("<INVALID COMMAND>");
      }
      item->handler(in, os, datasets, iterators);
    } catch (const std::exception &) {
      os << "<INVALID COMMAND>\n";
      in.clear();
      in.ignore(
        std::numeric_limits< std::streamsize >::max(),
        '\n'
      );
    }
  }
}

}
