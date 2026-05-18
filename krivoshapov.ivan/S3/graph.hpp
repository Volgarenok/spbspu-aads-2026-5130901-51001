#ifndef KRIVOSHAPOV_GRAPH_HPP
#define KRIVOSHAPOV_GRAPH_HPP

#include <string>
#include <utility>
#include "dynamic_array.hpp"
#include "hash_table.hpp"
#include "sha1.hpp"

namespace krivoshapov
{
  class Graph
  {
  public:
    using Weights = Array<size_t>;
    using NameList = Array<std::string>;

    Graph() : vertices_(8), edges_(8) {}

    void addVertex(const std::string &name)
    {
      if (!vertices_.has(name))
      {
        growVertices();
        vertices_.add(name, 1);
      }
    }

    bool hasVertex(const std::string &name) const
    {
      return vertices_.has(name);
    }

    void addEdge(const std::string &a, const std::string &b, size_t w)
    {
      addVertex(a);
      addVertex(b);
      VertexPair key(a, b);
      if (edges_.has(key))
      {
        edges_.at(key).push_back(w);
      }
      else
      {
        growEdges();
        Weights ws;
        ws.push_back(w);
        edges_.add(key, ws);
      }
    }

  private:
    using VertexSet = HashTable<std::string, char, StringHash, StringEqual>;
    using EdgeMap = HashTable<VertexPair, Weights, PairHash, PairEqual>;

    VertexSet vertices_;
    EdgeMap edges_;

    void growVertices()
    {
      if (vertices_.size() * 10 >= vertices_.slotCount() * 7)
      {
        vertices_.rehash(vertices_.slotCount() * 2);
      }
    }

    void growEdges()
    {
      if (edges_.size() * 10 >= edges_.slotCount() * 7)
      {
        edges_.rehash(edges_.slotCount() * 2);
      }
    }
    bool cutEdge(const std::string &a, const std::string &b, size_t w)
    {
      if (!vertices_.has(a) || !vertices_.has(b))
      {
        return false;
      }
      VertexPair key(a, b);
      if (!edges_.has(key))
      {
        return false;
      }
      Weights &ws = edges_.at(key);
      size_t found = ws.size();
      for (size_t i = 0; i < ws.size(); ++i)
      {
        if (ws[i] == w)
        {
          found = i;
          break;
        }
      }
      if (found == ws.size())
      {
        return false;
      }
      Weights nw;
      for (size_t i = 0; i < ws.size(); ++i)
      {
        if (i != found)
        {
          nw.push_back(ws[i]);
        }
      }
      if (nw.empty())
      {
        edges_.drop(key);
      }
      else
      {
        ws = nw;
      }
      return true;
    }
  };
}

#endif
