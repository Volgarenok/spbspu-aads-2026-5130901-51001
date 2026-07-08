#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <iostream>
#include "graph.hpp"
#include "../common/io_utils.hpp"

namespace hachaturyanov
{
  using GraphTable = HashTable< std::string, Graph,
        xxhash< std::string >, std::equal_to< std::string > >;

  void readFile(std::istream &in, GraphTable &graphs);
  void cmdGraphs(const GraphTable &graphs, std::ostream &out);
  void cmdVertexes(const GraphTable &graphs, const std::string &graphName, std::ostream &out);
  void cmdInOutbound(const GraphTable &graphs, const std::string &graphName,
        const std::string &vertexName, bool outbound, std::ostream &out);
  void cmdBind(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out);
  void cmdCut(GraphTable &graphs, const std::string &graph, const std::string &vertex1,
        const std::string &vertex2, size_t weight, std::ostream &out);
  void cmdCreate(GraphTable &graphs, const std::string &graph, size_t n,
        List< std::string > &vertices, std::ostream &out);
  void cmdMerge(GraphTable &graphs, std::string &newgraph,
        std::string &graph1, std::string &graph2, std::ostream &out);
  void cmdExtract(GraphTable &graphs, std::string &newgraph, std::string &oldgraph,
        size_t n, List< std::string > &vertices, std::ostream &out);
  void process(std::istream &in, std::ostream &out, GraphTable &graphs);
}

#endif
