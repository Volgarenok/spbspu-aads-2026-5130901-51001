#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <iostream>

#include "hash-table.hpp"
#include "sha1.hpp"
#include "list.hpp"

namespace shevchenko
{
using Edge = std::pair< std::string, std::string >;

struct EdgeHash
{
  size_t operator()(const Edge& edge) const
  {
    SHA1Hasher hash;
    
    return hash(edge.first) ^ hash(edge.second);
  }
};

class Graph
{
public:
  using WeightList = List< size_t >;
  
  void bind(
            const std::string& from,
            const std::string& to,
            size_t weight
            )
  {
    addVertex(from);
    addVertex(to);
    
    Edge edge(from, to);
    
    if (!edges_.contains(edge))
    {
      edges_.insert(edge, WeightList());
    }
    
    edges_.at(edge).pushBack(weight);
  }
  
  bool hasVertex(const std::string& vertex) const
  {
    for (auto it = vertexes_.begin();
         it != vertexes_.end();
         ++it)
    {
      if (*it == vertex)
      {
        return true;
      }
    }
    
    return false;
  }
  
  void printVertexes(std::ostream& out) const
  {
    for (auto it = vertexes_.begin();
         it != vertexes_.end();
         ++it)
    {
      out << *it << '\n';
    }
  }
  
private:
  void addVertex(const std::string& vertex)
  {
    if (!hasVertex(vertex))
    {
      vertexes_.pushBack(vertex);
    }
  }
  
  HashTable<Edge, WeightList, EdgeHash> edges_;
  
  List< std::string > vertexes_;
};
}

#endif
