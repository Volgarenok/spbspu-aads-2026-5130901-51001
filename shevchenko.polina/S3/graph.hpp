#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <stdexcept>

#include "list.hpp"
#include "hash-table.hpp"
#include "sha1.hpp"

namespace shevchenko
{
struct Edge
{
  std::string to;
  size_t weight;
  
  Edge():
  to(),
  weight(0)
  {
  }
  
  Edge(const std::string& dest, size_t w):
  to(dest),
  weight(w)
  {
  }
};

class Graph
{
public:
  using EdgeList = List< Edge >;
  
  using Table =
  HashTable<
  std::string,
  EdgeList,
  SHA1Hasher
  >;
  
  Graph():
  adjacency_()
  {
  }
  
  bool empty() const
  {
    return adjacency_.empty();
  }
  
  bool hasVertex(const std::string& vertex) const
  {
    return adjacency_.contains(vertex);
  }
  
  void addVertex(const std::string& vertex)
  {
    if (!hasVertex(vertex))
    {
      adjacency_.insert(vertex, EdgeList());
    }
  }
  
  void bind(const std::string& from, const std::string& to, size_t weight)
  {
    addVertex(from);
    addVertex(to);
    
    EdgeList& edges = adjacency_.at(from);
    
    edges.pushBack(Edge(to, weight));
  }
  
  void cut(const std::string& from, const std::string& to)
  {
    if (!hasVertex(from))
    {
      throw std::out_of_range("vertex not found");
    }
    
    EdgeList& edges = adjacency_.at(from);
    
    for (auto it = edges.begin(); it != edges.end();)
    {
      if ((*it).to == to)
      {
        it = edges.erase(it);
      }
      else
      {
        ++it;
      }
    }
  }
  
  const EdgeList& outbound(const std::string& vertex) const
  {
    return adjacency_.at(vertex);
  }
  
  EdgeList inbound(const std::string& vertex) const
  {
    EdgeList result;
    
    for (auto it = adjacency_.cbegin(); it != adjacency_.cend(); ++it)
    {
      const std::string& from = it->first;
      
      const EdgeList& edges = it->second;
      
      for (auto edge_it = edges.begin(); edge_it != edges.end(); ++edge_it)
      {
        if ((*edge_it).to == vertex)
        {
          result.pushBack(Edge(from, (*edge_it).weight));
        }
      }
    }
    
    return result;
  }
  
  void merge(const Graph& other)
  {
    for (auto it = other.adjacency_.cbegin(); it != other.adjacency_.cend(); ++it)
    {
      const std::string& vertex = it->first;
      
      addVertex(vertex);
      
      const EdgeList& edges = it->second;
      
      for (auto edge_it = edges.begin(); edge_it != edges.end(); ++edge_it)
      {
        bind(vertex, (*edge_it).to, (*edge_it).weight);
      }
    }
  }
  
  Graph extract(const std::string& vertex) const
  {
    Graph result;
    
    if (!hasVertex(vertex))
    {
      return result;
    }
    
    result.addVertex(vertex);
    
    const EdgeList& edges = adjacency_.at(vertex);
    
    for (auto it = edges.begin(); it != edges.end(); ++it)
    {
      result.bind(vertex, (*it).to, (*it).weight);
    }
    
    return result;
  }
  
  const Table& data() const
  {
    return adjacency_;
  }
  
private:
  Table adjacency_;
};
}

#endif
