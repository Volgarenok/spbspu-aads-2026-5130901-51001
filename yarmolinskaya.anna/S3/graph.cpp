#include "graph.hpp"

#include <algorithm>
#include <stdexcept>

bool yarmolinskaya::Edge::operator==(
  const Edge& rhs
) const
{
  return
    from == rhs.from &&
    to == rhs.to;
}

size_t yarmolinskaya::EdgeHash::operator()(
  const Edge& edge
) const
{
  StringHash hash;

  return
    hash(edge.from) * 31 +
    hash(edge.to);
}

yarmolinskaya::Graph::Graph():
  vertexes_(),
  edges_(53, 2, 53)
{}

bool yarmolinskaya::Graph::hasVertex(
  const std::string& vertex
) const
{
  return vertexes_.contains(vertex);
}

bool yarmolinskaya::Graph::hasEdge(
  const std::string& from,
  const std::string& to
) const
{
  return edges_.contains(
    Edge{from, to}
  );
}

void yarmolinskaya::Graph::addVertex(
  const std::string& vertex
)
{
  if (!hasVertex(vertex))
  {
    vertexes_.push_back(vertex);
  }
}

void yarmolinskaya::Graph::bind(
  const std::string& from,
  const std::string& to,
  size_t weight
)
{
  Graph backup(*this);

  try
  {
    addVertex(from);
    addVertex(to);

    Edge edge{from, to};

    if (!edges_.contains(edge))
    {
      WeightList weights;

      weights.push_back(weight);

      edges_.insert(edge, weights);
    }
    else
    {
      edges_.at(edge).push_back(weight);
    }
  }
  catch (...)
  {
    swap(backup);

    throw;
  }
}

bool yarmolinskaya::Graph::cut(
  const std::string& from,
  const std::string& to,
  size_t weight
)
{
  Edge edge{from, to};

  if (!edges_.contains(edge))
  {
    return false;
  }

  WeightList updated;

  bool removed = false;

  for (size_t current : edges_.at(edge))
  {
    if (
      !removed &&
      current == weight
    )
    {
      removed = true;

      continue;
    }

    updated.push_back(current);
  }

  if (!removed)
  {
    return false;
  }

  if (updated.empty())
  {
    edges_.erase(edge);
  }
  else
  {
    edges_.at(edge) = updated;
  }

  return true;
}

std::vector< std::string >
yarmolinskaya::Graph::vertexes() const
{
  std::vector< std::string > result;

  for (const std::string& vertex : vertexes_)
  {
    result.push_back(vertex);
  }

  std::sort(
    result.begin(),
    result.end()
  );

  return result;
}

yarmolinskaya::Graph::WeightVector
yarmolinskaya::Graph::toVector(
  const WeightList& weights
) const
{
  WeightVector result;

  for (size_t weight : weights)
  {
    result.push_back(weight);
  }

  std::sort(
    result.begin(),
    result.end()
  );

  return result;
}

yarmolinskaya::Graph::AdjacencyList
yarmolinskaya::Graph::collectEdges(
  const std::string& vertex,
  bool outboundDirection
) const
{
  AdjacencyList result;

  for (
    EdgeTable::const_iterator it =
      edges_.begin();
    it != edges_.end();
    ++it
  )
  {
    bool matches =
      outboundDirection ?
      (it->first.from == vertex) :
      (it->first.to == vertex);

    if (matches)
    {
      std::string adjacent =
        outboundDirection ?
        it->first.to :
        it->first.from;

      result.push_back(
        {
          adjacent,
          toVector(it->second)
        }
      );
    }
  }

  std::sort(
    result.begin(),
    result.end(),
    [](const AdjacentVertex& lhs,
       const AdjacentVertex& rhs)
    {
      return lhs.first < rhs.first;
    }
  );

  return result;
}

yarmolinskaya::Graph::AdjacencyList
yarmolinskaya::Graph::outbound(
  const std::string& vertex
) const
{
  if (!hasVertex(vertex))
  {
    throw std::logic_error(
      "invalid vertex"
    );
  }

  return collectEdges(vertex, true);
}

yarmolinskaya::Graph::AdjacencyList
yarmolinskaya::Graph::inbound(
  const std::string& vertex
) const
{
  if (!hasVertex(vertex))
  {
    throw std::logic_error(
      "invalid vertex"
    );
  }

  return collectEdges(vertex, false);
}

yarmolinskaya::Graph
yarmolinskaya::Graph::merge(
  const Graph& lhs,
  const Graph& rhs
)
{
  Graph result(lhs);

  for (const std::string& vertex : rhs.vertexes_)
  {
    result.addVertex(vertex);
  }

  for (
    EdgeTable::const_iterator it =
      rhs.edges_.begin();
    it != rhs.edges_.end();
    ++it
  )
  {
    for (size_t weight : it->second)
    {
      result.bind(
        it->first.from,
        it->first.to,
        weight
      );
    }
  }

  return result;
}

yarmolinskaya::Graph
yarmolinskaya::Graph::extract(
  const Graph& graph,
  const std::vector< std::string >& vertexes
)
{
  Graph result;

  for (const std::string& vertex : vertexes)
  {
    if (!graph.hasVertex(vertex))
    {
      throw std::logic_error(
        "invalid vertex"
      );
    }

    result.addVertex(vertex);
  }

  for (
    EdgeTable::const_iterator it =
      graph.edges_.begin();
    it != graph.edges_.end();
    ++it
  )
  {
    bool fromExists = false;
    bool toExists = false;

    for (const std::string& vertex : vertexes)
    {
      if (vertex == it->first.from)
      {
        fromExists = true;
      }

      if (vertex == it->first.to)
      {
        toExists = true;
      }
    }

    if (fromExists && toExists)
    {
      for (size_t weight : it->second)
      {
        result.bind(
          it->first.from,
          it->first.to,
          weight
        );
      }
    }
  }

  return result;
}

void yarmolinskaya::Graph::swap( Graph& rhs) noexcept
{
  vertexes_.swap(rhs.vertexes_);

  edges_.swap(rhs.edges_);
}
