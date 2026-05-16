#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <cstddef>
#include <string>

namespace muraviev
{
  struct StringHash
  {
    size_t operator()(const std::string& value) const;
  };

  struct StringEqual
  {
    bool operator()(const std::string& lhs, const std::string& rhs) const;
  };

  struct EdgeKey
  {
    std::string from;
    std::string to;
  };

  struct EdgeKeyHash
  {
    size_t operator()(const EdgeKey& value) const;
  };

  struct EdgeKeyEqual
  {
    bool operator()(const EdgeKey& lhs, const EdgeKey& rhs) const;
  };
}

#endif
