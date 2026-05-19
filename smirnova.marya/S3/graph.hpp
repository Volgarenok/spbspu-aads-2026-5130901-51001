#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hashTable.hpp"
#include "../common/vector.hpp"
#include <string>

namespace smirnova
{

class Graph
  {
  public:
    struct Edge
    {
      std::string to;
      Vector< int > weights;
    };
    HashTable< std::string, Vector< Edge > > adj;
    void addVertex(const std::string& v);
    void addEdge(const std::string& a,const std::string& b, int w);
    void cut(const std::string& a, const std::string& b, int w);
  };

}

#endif

