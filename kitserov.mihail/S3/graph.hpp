#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash.hpp"
#include <vector>
#include <algorithm>
#include <set>

namespace kitserov
{
  class Graph
  {
    std::string name_;
    HashTable< 
      std::pair<std::string, std::string>,
      std::vector<size_t>,
      PairHash<std::string, std::string, SipHash>,
      std::equal_to< std::pair<std::string, std::string> >
    > edges_;
    std::set< std::string > vertices_;
  public:

    Graph() = default;
    explicit Graph(const std::string& n) : name_(n) {}
    
    void addEdge(const std::string& src, const std::string& dst, size_t weight)
    {
      vertices_.insert(src);
      vertices_.insert(dst);
      auto* vec = edges_.find({src, dst});
      if (vec) {
        vec->push_back(weight);
      } else {
        edges_.add({src, dst}, {weight});
      }
    }

    bool cut(const std::string& src, const std::string& dst, size_t weight)
    {
      std::vector< size_t >* point = edges_.find({src, dst});
      if (!point) return false;
      auto weightIt = std::find(point -> begin(), point -> end(), weight);
      if (weightIt == point -> end()) return false;
      point -> erase(weightIt);
      if (point -> empty()) {
        edges_.erase({src, dst});
      }
      return true;
    }
    bool hasVertex(const std::string& v) const
    {
      return vertices_.find(v) != vertices_.end();
    }

    std::vector< std::string > getVertices() const
    {
      std::vector< std::string > v(vertices_.begin(), vertices_.end());
      std::sort(v.begin(), v.end());
      return v;
    }

    std::vector< std::pair< std::string, std::vector< size_t > > > getOutbound(const std::string& src) const
    {
      std::vector< std::pair< std::string, std::vector< size_t > > > res;
      for (const auto& dst : getVertices()) {
        std::vector< size_t >* weights = const_cast< std::vector< size_t >* >(edges_.find({src, dst}));
        if (weights) {
          std::vector< size_t > w(weights -> begin(), weights -> end());
          std::sort(w.begin(), w.end());
          for (const auto& weight : w) {
            res.push_back({dst, {weight}});
          }
        }
      }
      return res;
    }

    std::vector< std::pair< std::string, std::vector< size_t > > > getInbound(const std::string& dst) const
    {
      std::vector< std::pair< std::string, std::vector< size_t > > > res;
      for (const auto& src : getVertices()) {
        std::vector< size_t >* weights = const_cast< std::vector< size_t >* >(edges_.find({src, dst}));
        if (weights) {
          std::vector< size_t > w(weights -> begin(), weights -> end());
          std::sort(w.begin(), w.end());
          for (const auto& weight : w) {
            res.push_back({src, {weight}});
          }
        }
      }
      return res;
    }

    Graph create(const std::string& name, const std::vector< std::string >& vertices)
    {
      Graph g(name);
      for (const auto& v : vertices) {
        g.vertices_.insert(v);
      }
      return g;
    }

    Graph merge(const std::string& name, const Graph& a, const Graph& b)
    {
      Graph g(name);
      g.vertices_ = a.vertices_;
      g.vertices_.insert(b.vertices_.begin(), b.vertices_.end());
      g.edges_ = std::move(a.edges_);
      for (auto it = b.edges_.begin(); it < b.edges_.end(); ++it) {
        g.edges_.add(it.key(), *it);
      }
      return g;
    }

    Graph extract(const Graph& src, const std::string& name, const std::vector<std::string>& vertices)
    {
      Graph g(name);
      for (const auto& v : vertices) {
        if (!src.hasVertex(v)) {
          throw std::invalid_argument("Vertex not found in source graph");
        }
        g.vertices_.insert(v);
      }
      for (auto it = src.edges_.begin(); it < src.edges_.end(); ++it) {
        auto key = it.key();
        const std::string& v1 = key.first;
        const std::string& v2 = key.second;
        if (g.vertices_.count(v1) && g.vertices_.count(v2)) {
          g.edges_.add(key, *it);
        }
      }
      return g;
    }
  };
}
#endif