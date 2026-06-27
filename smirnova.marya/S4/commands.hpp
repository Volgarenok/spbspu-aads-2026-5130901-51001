#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <sstream>
#include <string>

#include "bstree.hpp"

namespace smirnova
{
  using Dict = BSTree< int, std::string, Less< int > >;
  using DatasetTree = BSTree< std::string, Dict, Less< std::string > >;

  void printDataset(const DatasetTree& datasets, const std::string& name);

  Dict complement(const Dict& left, const Dict& right);

  Dict intersect(const Dict& left, const Dict& right);

  Dict unionDict(const Dict& left, const Dict& right);

  void requireNoExtra(std::istringstream& iss);

  void executeCommand(DatasetTree& datasets, std::istringstream& input);
}

#endif

