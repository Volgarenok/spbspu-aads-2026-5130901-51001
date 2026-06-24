#ifndef YARMOLINSKAYA_GRAPH_HPP
#define YARMOLINSKAYA_GRAPH_HPP

#include "hash_table.hpp"
#include "list.hpp"

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace yarmolinskaya
{
  struct Edge
  {
    std::string from;
    std::string to;

    bool operator==(const Edge& rhs) const;
  };

  struct EdgeHash
  {
    size_t operator()(const Edge& edge) const;
  };

  class Graph
  {
  public:
    using WeightList =
      List< size_t >;

    using WeightVector =
      std::vector< size_t >;

    using AdjacentVertex =
      std::pair<
        std::string,
        WeightVector
      >;

    using AdjacencyList =
      std::vector< AdjacentVertex >;

    using EdgeTable =
      HashTable<
        Edge,
        WeightList,
        EdgeHash
      >;

    Graph();

    bool hasVertex(
      const std::string& vertex
    ) const;

    bool hasEdge(
      const std::string& from,
      const std::string& to
    ) const;

    void addVertex(
      const std::string& vertex
    );

    void bind(
      const std::string& from,
      const std::string& to,
      size_t weight
    );

    bool cut(
      const std::string& from,
      const std::string& to,
      size_t weight
    );

    std::vector< std::string >
    vertexes() const;

    AdjacencyList outbound(
      const std::string& vertex
    ) const;

    AdjacencyList inbound(
      const std::string& vertex
    ) const;

    static Graph merge(
      const Graph& lhs,
      const Graph& rhs
    );

    static Graph extract(
      const Graph& graph,
      const std::vector< std::string >& vertexes
    );

  private:
    List< std::string > vertexes_;

    EdgeTable edges_;

    void swap(Graph& rhs) noexcept;

    WeightVector toVector(
      const WeightList& weights
    ) const;

    AdjacencyList collectEdges(
      const std::string& vertex,
      bool outboundDirection
    ) const;
  };
}

#endif
