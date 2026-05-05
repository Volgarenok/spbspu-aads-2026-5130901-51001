#ifndef ALEKSEEV_S3_GRAPH_HPP
#define ALEKSEEV_S3_GRAPH_HPP

#include <string>

#include "edge_key.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"

namespace alekseev
{
  using EdgeTable = HashTable< EdgeKey, Sequence< unsigned long long >, HmacHash, EdgeKeyEqual >;

  struct EdgeQueryLine
  {
    std::string vertex;
    Sequence< unsigned long long > weights;
  };

  class Graph
  {
  public:
    Graph():
      vertexes_(),
      edges_(211)
    {}

    explicit Graph(size_t expected_edges):
      vertexes_(),
      edges_(next_odd_capacity(expected_edges * 2 + 3))
    {}

    void add_vertex(const std::string& name)
    {
      if (!has_vertex(name))
      {
        vertexes_.push_back(name);
      }
    }

    bool has_vertex(const std::string& name) const
    {
      return contains_vertex(vertexes_, name);
    }

    void bind(const std::string& from, const std::string& to, unsigned long long weight)
    {
      add_vertex(from);
      add_vertex(to);
      EdgeKey key{from, to};
      if (edges_.has(key))
      {
        edges_.at(key).push_back(weight);
      }
      else
      {
        Sequence< unsigned long long > weights;
        weights.push_back(weight);
        edges_.add(key, weights);
      }
    }

    bool cut(const std::string& from, const std::string& to, unsigned long long weight)
    {
      if (!has_vertex(from) || !has_vertex(to))
      {
        return false;
      }
      EdgeKey key{from, to};
      if (!edges_.has(key))
      {
        return false;
      }
      Sequence< unsigned long long >& weights = edges_.at(key);
      for (size_t i = 0; i < weights.size(); ++i)
      {
        if (weights[i] == weight)
        {
          weights.erase_at(i);
          if (weights.empty())
          {
            edges_.drop(key);
          }
          return true;
        }
      }
      return false;
    }

    Sequence< std::string > vertexes_sorted() const
    {
      Sequence< std::string > result(vertexes_);
      sort(result, StringLess());
      return result;
    }

    Sequence< EdgeQueryLine > outbound_sorted(const std::string& vertex) const
    {
      Sequence< EdgeQueryLine > result;
      for (typename EdgeTable::const_iterator it = edges_.begin(); it != edges_.end(); ++it)
      {
        if (it->key().from == vertex)
        {
          append_query_line(result, it->key().to, it->value());
        }
      }
      sort_query_lines(result);
      return result;
    }

    Sequence< EdgeQueryLine > inbound_sorted(const std::string& vertex) const
    {
      Sequence< EdgeQueryLine > result;
      for (typename EdgeTable::const_iterator it = edges_.begin(); it != edges_.end(); ++it)
      {
        if (it->key().to == vertex)
        {
          append_query_line(result, it->key().from, it->value());
        }
      }
      sort_query_lines(result);
      return result;
    }

    const EdgeTable& edges() const
    {
      return edges_;
    }

  private:
    struct StringLess
    {
      bool operator()(const std::string& lhs, const std::string& rhs) const
      {
        return lhs < rhs;
      }
    };

    struct ULLLess
    {
      bool operator()(unsigned long long lhs, unsigned long long rhs) const
      {
        return lhs < rhs;
      }
    };

    struct QueryLineLess
    {
      bool operator()(const EdgeQueryLine& lhs, const EdgeQueryLine& rhs) const
      {
        return lhs.vertex < rhs.vertex;
      }
    };

    static size_t next_odd_capacity(size_t value)
    {
      if (value < 11)
      {
        return 11;
      }
      return value % 2 == 0 ? value + 1 : value;
    }

    static bool contains_vertex(const Sequence< std::string >& seq, const std::string& name)
    {
      for (size_t i = 0; i < seq.size(); ++i)
      {
        if (seq[i] == name)
        {
          return true;
        }
      }
      return false;
    }

    static void append_query_line(Sequence< EdgeQueryLine >& result, const std::string& vertex,
        const Sequence< unsigned long long >& weights)
    {
      EdgeQueryLine line;
      line.vertex = vertex;
      line.weights = weights;
      sort(line.weights, ULLLess());
      result.push_back(line);
    }

    static void sort_query_lines(Sequence< EdgeQueryLine >& lines)
    {
      sort(lines, QueryLineLess());
    }

    Sequence< std::string > vertexes_;
    EdgeTable edges_;
  };
}

#endif
