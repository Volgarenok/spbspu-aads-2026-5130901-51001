#ifndef S4_IO_HPP
#define S4_IO_HPP

#include <bstree.hpp>

#include <iosfwd>
#include <string>

namespace shaykhraziev
{
  struct IntLess
  {
    bool operator()(int lhs, int rhs) const;
  };

  struct StringLess
  {
    bool operator()(const std::string& lhs, const std::string& rhs) const;
  };

  using Dictionary = BSTree< int, std::string, IntLess >;
  using DatasetTable = BSTree< std::string, Dictionary, StringLess >;

  bool parseInt(const std::string& value, int& result);
  DatasetTable readDatasets(std::istream& in);
  DatasetTable readDatasetsFromFile(const char* filename);
}

#endif
