#include "graph.hpp"

#include "sip_hash.hpp"

size_t muraviev::StringHash::operator()(const std::string& value) const
{
  const unsigned char* data = reinterpret_cast< const unsigned char* >(value.data());
  return static_cast< size_t >(sipHash24(data, value.size()));
}

bool muraviev::StringEqual::operator()(const std::string& lhs,
    const std::string& rhs) const
{
  return lhs == rhs;
}

size_t muraviev::EdgeKeyHash::operator()(const EdgeKey& value) const
{
  std::string bytes = value.from;
  bytes += '\0';
  bytes += value.to;
  const unsigned char* data = reinterpret_cast< const unsigned char* >(bytes.data());
  return static_cast< size_t >(sipHash24(data, bytes.size()));
}

bool muraviev::EdgeKeyEqual::operator()(const EdgeKey& lhs, const EdgeKey& rhs) const
{
  return lhs.from == rhs.from && lhs.to == rhs.to;
}

muraviev::Graph::Graph(const std::string& name):
  name_(name),
  edges_(53),
  vertexes_(53)
{}

void muraviev::Graph::addVertex(const std::string& vertex)
{
  vertexes_.add(vertex, true);
}

bool muraviev::Graph::hasVertex(const std::string& vertex) const
{
  return vertexes_.has(vertex);
}

void muraviev::Graph::addEdge(const std::string& from, const std::string& to,
    unsigned long long weight)
{
  addVertex(from);
  addVertex(to);
  EdgeKey key = {from, to};
  WeightList weights;
  if (edges_.has(key)) {
    weights = edges_.at(key);
  }
  if (weights.empty()) {
    weights.pushFront(weight);
  } else {
    weights.insert(weights.last(), weight);
  }
  edges_.add(key, weights);
}
