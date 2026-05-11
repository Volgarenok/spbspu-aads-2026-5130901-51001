#include "hash.hpp"
#include <vector>

namespace kitserov
{
  class Graph
  {
    std::string name_;
    HashTable< std::pair< std::string, std::string >, std::vector< size_t >, PairHash< std::string, std::string > > edges_;
    
    Graph() = default;
    explicit Graph(const std::string& n) : name_(n) {}

    void addEdge(const std::string& src, const std::string& dst, size_t weight) {
      edges[{src, dst}].push_back(weight);
    }
  };
}
