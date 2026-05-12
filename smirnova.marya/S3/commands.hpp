#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include "graph.hpp"
#include <iostream>

namespace smirnova
{

  HashTable<std::string, Graph> graphs;

  void graphs_cmd(std::ostream& out);

  void create(std::istream& in, std::ostream& out, std::string graphName);
  void bind(std::istream& in, std::ostream& out, std::string graphName);
  void cut(std::istream& in, std::ostream& out, std::string graphName);

  void outbound(std::istream& in, std::ostream& out, std::string graphName);
  void inbound(std::istream& in, std::ostream& out, std::string graphName);

  void merge(std::istream& in, std::ostream& out, std::string graphName);
  void extract(std::istream& in, std::ostream& out, std::string graphName);

}

#endif

