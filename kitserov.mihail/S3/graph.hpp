#include "hash.hpp"
#include <vector>
#include <algorithm>

namespace kitserov
{
  class Graph
  {
    std::string name_;
    HashTable< std::pair< std::string, std::string >, std::vector< size_t >, PairHash< std::string, std::string > > edges_;
    std::set< std::string > vertices_;
  public:

    Graph() = default;
    explicit Graph(const std::string& n) : name_(n) {}
    
    void addEdge(const std::string& src, const std::string& dst, size_t weight)
    {
      vertices_.insert(src);
      vertices_.insert(dst);
      edges[{src, dst}].push_back(weight);
    }

    bool cut(const std::string& src, const std::string& dst, size_t weight)
    {
      std::vector< size_t >* point = (edges_.find({src, dst}));
      if (!point) return false;
      auto weightIt = std::find(point -> begin(), point -> end(), weight);
      if (weightIt == point -> end()) return false;
      (*(point)).erase(weightIt);
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
        std::vector< size_t >* weights = edges_.find({src, dst});
        if (weights) {
          std::vector< size_t > w(weights -> begin(), weights -> end());
          std::sort(w.begin(), w.end());
          for (const auto& weight : w) {
            res.push_back({dst, weight});
          }
        }
      }
      return res;
    }

    
  };
}
