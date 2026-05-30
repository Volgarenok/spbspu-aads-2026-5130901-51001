#include "graph.hpp"

#include <algorithm>
#include <stdexcept>

namespace loseva {

Graph::Graph(std::size_t edgeCapacity):
  edges_(edgeCapacity),
  vertices_()
{}

void Graph::addVertex(const std::string & v)
{
  vertices_.insert(v);
}

void Graph::addEdge(
  const std::string & from,
  const std::string & to,
  unsigned int weight)
{
  vertices_.insert(from);
  vertices_.insert(to);
  const EdgeKey key = { from, to };
  if (edges_.has(key)) {
    edges_.get(key).push_back(weight);
  } else {
    EdgeWeights w = { weight };
    bool inserted = false;
    while (!inserted) {
      try {
        edges_.add(key, w);
        inserted = true;
      } catch (const TableFullException &) {
        edges_.rehash(edges_.capacity() * 2 + 1);
      }
    }
  }
}

bool Graph::removeEdge(
  const std::string & from,
  const std::string & to,
  unsigned int weight)
{
  const EdgeKey key = { from, to };
  if (!edges_.has(key)) {
    return false;
  }
  auto & weights = edges_.get(key);
  const auto it = std::find(weights.begin(), weights.end(), weight);
  if (it == weights.end()) {
    return false;
  }
  weights.erase(it);
  if (weights.empty()) {
    edges_.drop(key);
  }
  return true;
}

bool Graph::hasVertex(const std::string & v) const
{
  return vertices_.find(v) != vertices_.end();
}

bool Graph::hasEdge(const std::string & from, const std::string & to) const
{
  return edges_.has({ from, to });
}

std::vector< std::string > Graph::sortedVertices() const
{
  return std::vector< std::string >(vertices_.begin(), vertices_.end());
}

std::vector< std::pair< std::string, std::vector< unsigned int > > >
Graph::outbound(const std::string & v) const
{
  std::vector< std::pair< std::string, std::vector< unsigned int > > > result;
  for (const auto & entry : edges_) {
    const EdgeKey & key = entry.first;
    const EdgeWeights & weights = entry.second;
    if (key.first == v) {
      std::vector< unsigned int > sorted = weights;
      std::sort(sorted.begin(), sorted.end());
      result.push_back({ key.second, sorted });
    }
  }
  std::sort(result.begin(), result.end(),
    [](const auto & a, const auto & b) {
      return a.first < b.first;
    });
  return result;
}

std::vector< std::pair< std::string, std::vector< unsigned int > > >
Graph::inbound(const std::string & v) const
{
  std::vector< std::pair< std::string, std::vector< unsigned int > > > result;
  for (const auto & entry : edges_) {
    const EdgeKey & key = entry.first;
    const EdgeWeights & weights = entry.second;
    if (key.second == v) {
      std::vector< unsigned int > sorted = weights;
      std::sort(sorted.begin(), sorted.end());
      result.push_back({ key.first, sorted });
    }
  }
  std::sort(result.begin(), result.end(),
    [](const auto & a, const auto & b) {
      return a.first < b.first;
    });
  return result;
}

const EdgeTable & Graph::edges() const
{
  return edges_;
}

const VertexSet & Graph::vertices() const
{
  return vertices_;
}

std::size_t Graph::edgeCapacity() const
{
  return edges_.capacity();
}

}
