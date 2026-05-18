#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <stdexcept>

#include "../common/list.hpp"
#include "hash_table.hpp"
#include "sha1.hpp"

namespace shevchenko
{

struct Edge
{
  std::string to;
  size_t weight;
  
  Edge();
  Edge(const std::string& dest, size_t w);
  
  bool operator==(const Edge& other) const;
};

class Graph
{
public:
  using EdgeList = List<Edge>;
  using Table = HashTable<std::string, EdgeList, SHA1Hasher>;
  
  Graph();
  
  Graph(const Graph& other) = default;
  Graph(Graph&& other) noexcept = default;
  Graph& operator=(const Graph& other) = default;
  Graph& operator=(Graph&& other) noexcept = default;
  
  bool empty() const;
  bool hasVertex(const std::string& vertex) const;
  void addVertex(const std::string& vertex);
  void bind(const std::string& from, const std::string& to, size_t weight);
  void cut(const std::string& from, const std::string& to, size_t weight);
  const EdgeList& outbound(const std::string& vertex) const;
  EdgeList inbound(const std::string& vertex) const;
  void merge(const Graph& other);
  Graph extract(const List<std::string>& vertices) const;
  const Table& data() const;
  
private:
  Table adjacency_;
};

}

#endif
