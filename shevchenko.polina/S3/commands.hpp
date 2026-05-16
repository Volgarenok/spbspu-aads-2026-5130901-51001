#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "graph.hpp"
#include "hash-table.hpp"
#include "sha1.hpp"

namespace shevchenko
{
using GraphTable = HashTable<std::string, Graph, SHA1Hasher>;

inline void printInvalid(std::ostream& out)
{
  out << "<INVALID COMMAND>\n";
}

inline void graphsCommand(const GraphTable& graphs, std::ostream& out)
{
  (void)graphs;
  
  out << "graphs\n";
}
}

#endif
