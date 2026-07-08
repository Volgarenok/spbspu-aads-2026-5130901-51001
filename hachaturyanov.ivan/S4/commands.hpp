#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "bstree.hpp"

#include <string>

namespace hachaturyanov
{
  using Dataset = BSTree< int, std::string, std::less< int > >;
  using Store = BSTree< std::string, Dataset, std::less< std::string > >;

  void readInput(Store &store, std::istream &in);
  void print(Store &store, std::string &name, std::ostream &out);
  void complement(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out);
  void intersect(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out);
  void unite(Store &store, std::string &newSet, std::string &old1, std::string &old2, std::ostream &out);
  void execute(Store &store, std::istream &in, std::ostream &out);
}

#endif
