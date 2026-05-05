#ifndef ALEKSEEV_S3_GRAPH_STORAGE_HPP
#define ALEKSEEV_S3_GRAPH_STORAGE_HPP

#include <string>

#include "graph.hpp"
#include "hash_table.hpp"
#include "hmac_hash.hpp"
#include "sequence.hpp"
#include "sort_utils.hpp"

namespace alekseev
{
  class GraphStorage
  {
  public:
    GraphStorage():
      graphs_(1009)
    {}

    bool has_graph(const std::string& name) const
    {
      return graphs_.has(name);
    }

    Graph& get_graph(const std::string& name)
    {
      return graphs_.at(name);
    }

    const Graph& get_graph(const std::string& name) const
    {
      return graphs_.at(name);
    }

    bool create_graph(const std::string& name)
    {
      if (has_graph(name))
      {
        return false;
      }
      graphs_.add(name, Graph());
      return true;
    }

    bool add_graph(const std::string& name, const Graph& graph)
    {
      if (has_graph(name))
      {
        return false;
      }
      graphs_.add(name, graph);
      return true;
    }

    Sequence< std::string > graph_names_sorted() const
    {
      Sequence< std::string > result;
      for (typename GraphTable::const_iterator it = graphs_.begin(); it != graphs_.end(); ++it)
      {
        result.push_back(it->key());
      }
      sort(result, StringLess());
      return result;
    }

  private:
    using GraphTable = HashTable< std::string, Graph, HmacHash, StringEqual >;

    struct StringLess
    {
      bool operator()(const std::string& lhs, const std::string& rhs) const
      {
        return lhs < rhs;
      }
    };

    GraphTable graphs_;
  };
}

#endif
