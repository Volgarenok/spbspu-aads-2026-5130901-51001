#include "graph.hpp"

namespace shevchenko
{

Edge::Edge()
: to()
, weight(0)
{
}

Edge::Edge(const std::string& dest, size_t w)
: to(dest)
, weight(w)
{
}

bool Edge::operator==(const Edge& other) const
{
  return to == other.to && weight == other.weight;
}

Graph::Graph()
: adjacency_()
{
}

bool Graph::empty() const
{
  return adjacency_.empty();
}

bool Graph::hasVertex(const std::string& vertex) const
{
  return adjacency_.contains(vertex);
}

void Graph::addVertex(const std::string& vertex)
{
  if (!hasVertex(vertex))
  {
    adjacency_.insert(vertex, EdgeList());
  }
}

void Graph::bind(const std::string& from, const std::string& to, size_t weight)
{
  addVertex(from);
  addVertex(to);
  
  EdgeList& edges = adjacency_.at(from);
  edges.pushBack(Edge(to, weight));
}

void Graph::cut(const std::string& from, const std::string& to, size_t weight)
{
  if (!hasVertex(from))
  {
    throw std::out_of_range("vertex not found");
  }
  
  EdgeList& edges = adjacency_.at(from);
  
  for (auto it = edges.begin(); it != edges.end();)
  {
    if (it->to == to && it->weight == weight)
    {
      it = edges.erase(it);
    }
    else
    {
      ++it;
    }
  }
}

const Graph::EdgeList& Graph::outbound(const std::string& vertex) const
{
  return adjacency_.at(vertex);
}

Graph::EdgeList Graph::inbound(const std::string& vertex) const
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

void Graph::merge(const Graph& other)
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

Graph Graph::extract(const std::string& vertex) const
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

const Graph::Table& Graph::data() const
{
  return adjacency_;
}

}
