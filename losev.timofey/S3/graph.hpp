#ifndef LOSEV_GRAPH_HPP
#define LOSEV_GRAPH_HPP

#include <string>
#include <utility>
#include <stdexcept>

#include "hash_table.hpp"
#include "../common/list.h"

namespace losev {

using VertexPair = std::pair<std::string, std::string>;
using WeightList = List<int>;

class Graph
{
public:
  explicit Graph(const std::string& name)
    : name_(name)
    , edges_(16)
    , vertices_(16)
  {}

  void addEdge(const std::string& from, const std::string& to, int weight)
  {
    addVertex(from);
    addVertex(to);
    
    VertexPair key(from, to);
    if (edges_.has(key)) {
      WeightList& weights = edges_.get(key);
      weights.push_front(weight);
    } else {
      WeightList weights;
      weights.push_front(weight);
      edges_.add(key, weights);
    }
  }

  void addVertex(const std::string& vertex)
  {
    if (!vertices_.has(vertex)) {
      vertices_.add(vertex, true);
    }
  }

  bool hasVertex(const std::string& vertex) const
  {
    return vertices_.has(vertex);
  }

  bool hasEdge(const std::string& from, const std::string& to) const
  {
    VertexPair key(from, to);
    return edges_.has(key);
  }

  const WeightList* getWeights(const std::string& from, const std::string& to) const
  {
    VertexPair key(from, to);
    if (edges_.has(key)) {
      return &edges_.get(key);
    }
    return nullptr;
  }

  const std::string& name() const
  {
    return name_;
  }

private:
  std::string name_;
  HashTable<VertexPair, WeightList, SipHash<VertexPair>, std::equal_to<VertexPair>> edges_;
  HashTable<std::string, bool, SipHash<std::string>, std::equal_to<std::string>> vertices_;
};

}

#endif
