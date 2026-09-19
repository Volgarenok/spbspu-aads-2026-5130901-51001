#ifndef LOSEVA_COMMANDS_HPP
#define LOSEVA_COMMANDS_HPP

#include "avl_tree.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

namespace loseva {

class CommandException : public std::runtime_error {
public:
  explicit CommandException(const std::string & msg = "<INVALID COMMAND>") :
    std::runtime_error(msg)
  {}
};

using Dataset = AVLTree< int >;

struct DatasetPair {
  std::string name;
  Dataset tree;

  bool operator<(const DatasetPair & rhs) const noexcept { return name < rhs.name; }
  bool operator>(const DatasetPair & rhs) const noexcept { return name > rhs.name; }
  bool operator==(const DatasetPair & rhs) const noexcept { return name == rhs.name; }
};

struct IteratorPair {
  std::string name;
  Dataset::const_iterator iter;

  bool operator<(const IteratorPair & rhs) const noexcept { return name < rhs.name; }
  bool operator>(const IteratorPair & rhs) const noexcept { return name > rhs.name; }
  bool operator==(const IteratorPair & rhs) const noexcept { return name == rhs.name; }
};

using DatasetsMap = AVLTree< DatasetPair >;
using IteratorsMap = AVLTree< IteratorPair >;

void cmdCreate(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdInsert(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRemove(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdSearch(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdPrint(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdTree(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdHeight(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdClear(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRotateLeft(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdRotateRight(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdMerge(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdContains(std::istream & is, std::ostream & os, DatasetsMap & dms);
void cmdBegin(std::istream & is, std::ostream & os, DatasetsMap & dms, IteratorsMap & ims);
void cmdNext(std::istream & is, std::ostream & os, IteratorsMap & ims);
void cmdIterator(std::istream & is, std::ostream & os, IteratorsMap & ims);

void processCommands(std::istream & in, std::ostream & out);

}

#endif
