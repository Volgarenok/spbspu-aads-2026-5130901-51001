#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>

#include "../common/list.hpp"
#include "hash_table.hpp"

namespace muraviev
{
  struct EdgeKey
  {
    std::string from;
    std::string to;
  };

  class Graph
  {
  public:
    explicit Graph(const std::string& name = "");
    const std::string& getName() const;

  private:
    std::string name_;
  };
}

#endif
