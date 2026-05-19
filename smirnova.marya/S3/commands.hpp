#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include <string>

#include "../common/vector.hpp"
#include "../common/list.hpp"
#include "graph.hpp"
#include "hashTable.hpp"

namespace smirnova
{
  using GraphTable = HashTable< std::string, Graph >;
  using VertTable  = HashTable< std::string, Vector< std::string > >;

  void graphs(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void vertexes(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void create(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void bind(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void cut(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void outbound(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void inbound(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void merge(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
  void extract(std::istream& in, std::ostream& out, GraphTable& graphs, VertTable& graphVertices, std::string graphName);
}

#endif

