#include "graph.hpp"
#include <algorithm>

namespace vishnyakov
{
  void Graph::add_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    if (!has_vertex(from))
    {
      vertices_.push_back(from);
    }

    if (!has_vertex(to))
    {
      vertices_.push_back(to);
    }

    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      edges_.add(key, List< Weight >());
    }

    List< Weight >& weights = edges_.at(key);
    weights.push_back(weight);
  }

  void Graph::remove_edge(const Vertex& from, const Vertex& to, Weight weight)
  {
    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      throw std::runtime_error("Edge not found");
    }

    List< Weight >& weights = edges_.at(key);

    if (weights.empty())
    {
      throw std::runtime_error("No edges found");
    }

    if (weights.front() == weight)
    {
      weights.pop_front();

      if (weights.empty())
      {
        edges_.drop(key);
      }

      return;
    }

    typename List< Weight >::Iter prev = weights.begin();
    typename List< Weight >::Iter it = weights.begin();
    ++it;

    for (; it != weights.end(); ++it)
    {
      if (*it == weight)
      {
        weights.erase_after(prev);

        if (weights.empty())
        {
          edges_.drop(key);
        }

        return;
      }

      prev = it;
    }

    throw std::runtime_error("Weight not found");
  }

  bool Graph::has_vertex(const Vertex& v) const
  {
    for (const Vertex& vertex : vertices_)
    {
      if (vertex == v)
      {
        return true;
      }
    }

    return false;
  }

  bool Graph::has_edge(const Vertex& from, const Vertex& to, Weight weight) const
  {
    EdgeKey key(from, to);

    if (!edges_.has(key))
    {
      return false;
    }

    const List< Weight >& weights = edges_.at(key);

    for (Weight w : weights)
    {
      if (w == weight)
      {
        return true;
      }
    }

    return false;
  }

  bool Graph::has_any_edge(const Vertex& from, const Vertex& to) const
  {
    EdgeKey key(from, to);
    return edges_.has(key);
  }

  bool Graph::empty() const
  {
    return vertices_.empty();
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_outbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;

    for (const auto& pair : edges_)
    {
      if (pair.first.first == v)
      {
        result.push_back(std::make_pair(pair.first.second, edges_.at(pair.first)));
      }
    }

    return result;
  }

  List< std::pair< Graph::Vertex, List< Graph::Weight > > > Graph::get_inbound(const Vertex& v) const
  {
    List< std::pair< Vertex, List< Weight > > > result;

    for (const auto& pair : edges_)
    {
      if (pair.first.second == v)
      {
        result.push_back(std::make_pair(pair.first.first, edges_.at(pair.first)));
      }
    }

    return result;
  }

  List< Graph::Vertex > Graph::get_sorted_vertices() const
  {
    List< Vertex > sorted;

    for (const Vertex& v : vertices_)
    {
      sorted.push_back(v);
    }

    for (typename List< Vertex >::Iter it = sorted.begin(); it != sorted.end(); ++it)
    {
      typename List< Vertex >::Iter min_it = it;

      for (typename List< Vertex >::Iter jt = it; jt != sorted.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }

      if (min_it != it)
      {
        Vertex tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    return sorted;
  }

  List< Graph::Weight > Graph::get_sorted_weights(const List< Weight >& weights) const
  {
    List< Weight > sorted;

    for (Weight w : weights)
    {
      sorted.push_back(w);
    }

    for (typename List< Weight >::Iter it = sorted.begin(); it != sorted.end(); ++it)
    {
      typename List< Weight >::Iter min_it = it;

      for (typename List< Weight >::Iter jt = it; jt != sorted.end(); ++jt)
      {
        if (*jt < *min_it)
        {
          min_it = jt;
        }
      }

      if (min_it != it)
      {
        Weight tmp = *it;
        *it = *min_it;
        *min_it = tmp;
      }
    }

    return sorted;
  }
}

