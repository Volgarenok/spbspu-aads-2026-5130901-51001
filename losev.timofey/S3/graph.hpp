#ifndef LOSEV_GRAPH_HPP
#define LOSEV_GRAPH_HPP

#include <string>
#include <utility>
#include <stdexcept>
#include <vector>
#include <algorithm>

#include "hash_table.hpp"
#include "sip_hash.hpp"
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

  bool cut(const std::string& from, const std::string& to, int weight)
  {
    VertexPair key(from, to);
    if (!edges_.has(key)) {
      return false;
    }

    WeightList weights = edges_.get(key);
    WeightList newWeights;
    bool found = false;

    for (auto it = weights.begin(); it != weights.end(); ++it) {
      if (!found && *it == weight) {
        found = true;
      } else {
        newWeights.push_front(*it);
      }
    }

    if (found) {
      edges_.drop(key);
      if (!newWeights.empty()) {
        edges_.add(key, newWeights);
      }
      return true;
    }
    return false;
  }

  const std::string& name() const
  {
    return name_;
  }

  std::vector<std::string> getVerticesSorted() const
  {
    std::vector<std::string> result;
    for (auto it = vertices_.begin(); it != vertices_.end(); ++it) {
      auto pair = *it;
      result.push_back(pair.first);
    }
    std::sort(result.begin(), result.end());
    return result;
  }

  std::vector<std::pair<std::string, std::vector<int>>> getOutbound(const std::string& vertex) const
  {
    std::vector<std::pair<std::string, std::vector<int>>> result;

    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      auto pair = *it;
      const VertexPair& key = pair.first;
      const WeightList& weightList = pair.second;

      if (key.first == vertex) {
        std::vector<int> weights;
        for (auto wit = weightList.begin(); wit != weightList.end(); ++wit) {
          weights.push_back(*wit);
        }
        std::sort(weights.begin(), weights.end());
        result.push_back({key.second, weights});
      }
    }

    std::sort(result.begin(), result.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

    return result;
  }

  std::vector<std::pair<std::string, std::vector<int>>> getInbound(const std::string& vertex) const
  {
    std::vector<std::pair<std::string, std::vector<int>>> result;

    for (auto it = edges_.begin(); it != edges_.end(); ++it) {
      auto pair = *it;
      const VertexPair& key = pair.first;
      const WeightList& weightList = pair.second;

      if (key.second == vertex) {
        std::vector<int> weights;
        for (auto wit = weightList.begin(); wit != weightList.end(); ++wit) {
          weights.push_back(*wit);
        }
        std::sort(weights.begin(), weights.end());
        result.push_back({key.first, weights});
      }
    }

    std::sort(result.begin(), result.end(),
      [](const auto& a, const auto& b) { return a.first < b.first; });

    return result;
  }

  static Graph create(const std::string& name, const std::vector<std::string>& vertices)
  {
    Graph g(name);
    for (const auto& v : vertices) {
      g.addVertex(v);
    }
    return g;
  }

  static Graph merge(const std::string& newName, const Graph& g1, const Graph& g2)
  {
    Graph result(newName);

    for (auto it = g1.edges_.begin(); it != g1.edges_.end(); ++it) {
      auto pair = *it;
      const VertexPair& key = pair.first;
      const WeightList& weights = pair.second;
      for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
        result.addEdge(key.first, key.second, *wit);
      }
    }

    for (auto it = g2.edges_.begin(); it != g2.edges_.end(); ++it) {
      auto pair = *it;
      const VertexPair& key = pair.first;
      const WeightList& weights = pair.second;
      for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
        result.addEdge(key.first, key.second, *wit);
      }
    }

    return result;
  }

  static Graph extract(const Graph& source, const std::string& newName,
                       const std::vector<std::string>& vertices)
  {
    Graph result(newName);

    for (const auto& v : vertices) {
      result.addVertex(v);
    }

    for (auto it = source.edges_.begin(); it != source.edges_.end(); ++it) {
      auto pair = *it;
      const VertexPair& key = pair.first;

      bool fromIn = false;
      bool toIn = false;

      for (const auto& v : vertices) {
        if (key.first == v) fromIn = true;
        if (key.second == v) toIn = true;
      }

      if (fromIn && toIn) {
        const WeightList& weights = pair.second;
        for (auto wit = weights.begin(); wit != weights.end(); ++wit) {
          result.addEdge(key.first, key.second, *wit);
        }
      }
    }

    return result;
  }

private:
  std::string name_;
  HashTable<VertexPair, WeightList, SipHash<VertexPair>, std::equal_to<VertexPair>> edges_;
  HashTable<std::string, bool, SipHash<std::string>, std::equal_to<std::string>> vertices_;
};

}

#endif
