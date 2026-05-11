#ifndef UTILS3_HPP
#define UTILS3_HPP

#include <vector>
#include <string>
#include <algorithm>
#include "graph.hpp"
namespace kitserov
{
  using HashGraphs = HashTable< std::string, Graph, SipHash< std::string >, std::equal_to< std::string > >;

  void cmd_graphs(std::ostream& out, std::istream&, HashGraphs& graphs) {
    std::vector< std::string > tmp;
    for (auto it = graphs.begin(); it != graphs.end(); ++it) {
      tmp.push_back(it.key());
    }
    std::sort(tmp.begin(), tmp.end());
    for (const auto& i : tmp) {
      out << i << "\n";
    }
  }
  void cmd_vertexes(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_outbound(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_inbound(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_bind(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_cut(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_create(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_merge(std::ostream& out, std::istream&, HashGraphs& graphs){}
  void cmd_extract(std::ostream& out, std::istream&, HashGraphs& graphs){}
}
#endif