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
  void cmd_vertexes(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName;
    if (!(in >> graphName)) {
      throw std::invalid_argument("invlid name");
    }
    Graph* graph = graphs.find(graphName);
    if (!graph) {
      throw std::invalid_argument("haven't this graph");
    }
    std::vector< std::string > verts = graph -> getVertices();
    for (const auto& v : verts) {
      out << v << '\n';
    }
  }
  void cmd_outbound(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName, vertex;
    if (!(in >> graphName >> vertex)) {
      throw std::invalid_argument("invlid name");
    }
    Graph* graph = graphs.find(graphName);
    if (!graph || !(graph -> hasVertex(vertex))) {
      throw std::invalid_argument("");
    }
    auto res = graph -> getOutbound(vertex);
    for (const auto& p : res) {
      out << p.first;
      for (size_t w : p.second) out << ' ' << w;
      out << '\n';
    }
  }
  void cmd_inbound(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName, vertex;
    if (!(in >> graphName >> vertex)) {
      throw std::invalid_argument("invlid name");
    }
    Graph* graph = graphs.find(graphName);
    if (!graph || !(graph -> hasVertex(vertex))) {
      throw std::invalid_argument("");
    }
    auto res = graph -> getInbound(vertex);
    for (const auto& p : res) {
      out << p.first;
      for (size_t w : p.second) out << ' ' << w;
      out << '\n';
    }
  }
  void cmd_bind(std::ostream&, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName, src, dst;
    size_t weight;
    if (!(in >> graphName >> src >> dst >> weight)) {
      throw std::invalid_argument("invlid name");
    }
    Graph* graph = graphs.find(graphName);
    if (!graph) {
      throw std::invalid_argument("");
    }
    graph -> addEdge(src, dst, weight);
  }
  void cmd_cut(std::ostream&, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName, src, dst;
    size_t weight;
    if (!(in >> graphName >> src >> dst >> weight)) {
      throw std::invalid_argument("invlid name");
    }
    Graph* graph = graphs.find(graphName);
    if (!graph) {
      throw std::invalid_argument("");
    }
    if (!(graph -> cut(src, dst, weight))) {
      throw std::runtime_error("cut failed");
    }
  }
  void cmd_create(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string graphName;
    size_t count;
    if (!(in >> graphName >> count)) {
      throw std::invalid_argument("invlid name");
    }
    if (graphs.find(graphName)) {
      throw std::invalid_argument("");
    }
    std::vector<std::string> verts(count);
    for (size_t i = 0; i < count; ++i) {
      if (!(in >> verts[i])) {
        throw std::invalid_argument("");
      }
    }
    graphs.add(graphName, Graph::create(graphName, verts));
  }
  void cmd_merge(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string newName, old1, old2;
    if (!(in >> newName >> old1 >> old2)) {
      throw std::invalid_argument("");
    }
    if (graphs.find(newName)) {
      throw std::invalid_argument("");
    }
    Graph* g1 = graphs.find(old1);
    Graph* g2 = graphs.find(old2);
    if (!g1 || !g2) {
      throw std::invalid_argument("");
    }
    graphs.add(newName, Graph::merge(newName, *g1, *g2));
  }
  void cmd_extract(std::ostream& out, std::istream& in, HashGraphs& graphs)
  {
    std::string newName, oldName;
    size_t count;
    if (!(in >> newName >> oldName >> count)) {
      throw std::invalid_argument("invlid name");
    }
    if (graphs.find(newName)) {
      throw std::invalid_argument("");
    }
    Graph* gOld = graphs.find(oldName);
    if (!gOld) {
      throw std::invalid_argument("invlid name");
    }
    std::vector<std::string> verts(count);
    for (size_t i = 0; i < count; ++i) {
      if (!(in >> verts[i])) {
        throw std::invalid_argument("");
      }
    }
    for (const auto& v : verts) {
      if (!(gOld -> hasVertex(v))) {
        throw std::invalid_argument("");
      }
    }
    graphs.add(newName, Graph::extract(*gOld, newName, verts));
  }
}
#endif