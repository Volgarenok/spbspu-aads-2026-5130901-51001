#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <vector>

#include "../common/list.hpp"
#include "hash_table.hpp"

namespace muraviev
{
  struct StringHash { size_t operator()(const std::string& value) const; };
  struct StringEqual { bool operator()(const std::string& lhs, const std::string& rhs) const; };
  struct EdgeKey { std::string from; std::string to; };
  struct EdgeKeyHash { size_t operator()(const EdgeKey& value) const; };
  struct EdgeKeyEqual { bool operator()(const EdgeKey& lhs, const EdgeKey& rhs) const; };

  class Graph
  {
  public:
    using WeightList = List< unsigned long long >;
    using EdgeTable = HashTable< EdgeKey, WeightList, EdgeKeyHash, EdgeKeyEqual >;
    using VertexTable = HashTable< std::string, bool, StringHash, StringEqual >;

    explicit Graph(const std::string& name = "");
    void addVertex(const std::string& vertex);
    bool hasVertex(const std::string& vertex) const;
    void addEdge(const std::string& from, const std::string& to, unsigned long long weight);

  private:
    std::string name_;
    EdgeTable edges_;
    VertexTable vertexes_;
  };
}

#endif
