#ifndef PARSER_HPP
#define PARSER_HPP

#include "graph.hpp"
#include "hash_table.hpp"
#include "xxhash.hpp"

#include <string>

namespace loseva {

using GraphTable = HashTable<
  std::string,
  Graph,
  StringXxHash,
  std::equal_to< std::string >
>;

void parseGraphFile(const std::string & filename, GraphTable & table);

}

#endif
