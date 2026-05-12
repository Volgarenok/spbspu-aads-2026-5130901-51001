#ifndef GRAPH_HPP
#define GRAPH_HPP

#include "hash_table.hpp"
#include "xxhash.hpp"

#include <set>
#include <string>
#include <vector>

namespace loseva {

using EdgeKey = std::pair< std::string, std::string >;
using EdgeWeights = std::vector< unsigned int >;

using EdgeTable = HashTable<
  EdgeKey,
  EdgeWeights,
  PairStringXxHash,
  PairStringEqual
>;

using VertexSet = std::set< std::string >;

class Graph {
public:
  Graph(std::size_t edgeCapacity = 64);

  Graph(const Graph &) = default;
  Graph(Graph &&) = default;
  Graph & operator=(const Graph &) = default;
  Graph & operator=(Graph &&) = default;

  void addVertex(const std::string & v);
  void addEdge(const std::string & from, const std::string & to,
    unsigned int weight);

  bool removeEdge(const std::string & from, const std::string & to,
    unsigned int weight);

  bool hasVertex(const std::string & v) const;
  bool hasEdge(const std::string & from, const std::string & to) const;

  std::vector< std::string > sortedVertices() const;

  std::vector< std::pair< std::string, std::vector< unsigned int > > >
    outbound(const std::string & v) const;

  std::vector< std::pair< std::string, std::vector< unsigned int > > >
    inbound(const std::string & v) const;

  const EdgeTable & edges() const;
  const VertexSet & vertices() const;

  std::size_t edgeCapacity() const;

private:
  EdgeTable edges_;
  VertexSet vertices_;
};

}

#endif
