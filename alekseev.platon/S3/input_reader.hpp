#ifndef ALEKSEEV_S3_INPUT_READER_HPP
#define ALEKSEEV_S3_INPUT_READER_HPP

#include <istream>
#include <limits>
#include <string>

#include "graph.hpp"
#include "graph_storage.hpp"

namespace alekseev
{
  namespace detail
  {
    inline bool is_digit(char c)
    {
      return c >= '0' && c <= '9';
    }

    inline bool is_alpha(char c)
    {
      return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    }

    inline bool is_alnum(char c)
    {
      return is_alpha(c) || is_digit(c);
    }

    inline bool is_valid_name(const std::string& value)
    {
      if (value.empty() || !is_alpha(value[0]))
      {
        return false;
      }
      for (size_t i = 1; i < value.size(); ++i)
      {
        if (!is_alnum(value[i]))
        {
          return false;
        }
      }
      return true;
    }

    inline bool parse_size(const std::string& value, size_t& result)
    {
      if (value.empty())
      {
        return false;
      }
      size_t current = 0;
      for (size_t i = 0; i < value.size(); ++i)
      {
        if (!is_digit(value[i]))
        {
          return false;
        }
        const size_t digit = static_cast< size_t >(value[i] - '0');
        if (current > (std::numeric_limits< size_t >::max() - digit) / 10)
        {
          return false;
        }
        current = current * 10 + digit;
      }
      result = current;
      return true;
    }

    inline bool parse_ull(const std::string& value, unsigned long long& result)
    {
      if (value.empty())
      {
        return false;
      }
      unsigned long long current = 0;
      for (size_t i = 0; i < value.size(); ++i)
      {
        if (!is_digit(value[i]))
        {
          return false;
        }
        const unsigned long long digit = static_cast< unsigned long long >(value[i] - '0');
        if (current > (std::numeric_limits< unsigned long long >::max() - digit) / 10)
        {
          return false;
        }
        current = current * 10 + digit;
      }
      result = current;
      return true;
    }

    inline bool split_exact(const std::string& line, Sequence< std::string >& words)
    {
      words.clear();
      if (line.empty())
      {
        return true;
      }
      size_t start = 0;
      while (start <= line.size())
      {
        size_t end = start;
        while (end < line.size() && line[end] != ' ')
        {
          ++end;
        }
        if (end == start)
        {
          return false;
        }
        words.push_back(line.substr(start, end - start));
        if (end == line.size())
        {
          return true;
        }
        start = end + 1;
      }
      return true;
    }
  }

  inline bool load_graphs(std::istream& input, GraphStorage& storage)
  {
    std::string line;
    Sequence< std::string > words;
    while (std::getline(input, line))
    {
      if (line.empty())
      {
        continue;
      }
      if (!detail::split_exact(line, words) || words.size() != 2 ||
          !detail::is_valid_name(words[0]))
      {
        return false;
      }
      size_t edge_count = 0;
      if (!detail::parse_size(words[1], edge_count))
      {
        return false;
      }

      const std::string graph_name = words[0];
      Graph graph(edge_count);
      for (size_t i = 0; i < edge_count; ++i)
      {
        if (!std::getline(input, line))
        {
          return false;
        }
        while (line.empty())
        {
          if (!std::getline(input, line))
          {
            return false;
          }
        }
        if (!detail::split_exact(line, words) || words.size() != 3 ||
            !detail::is_valid_name(words[0]) || !detail::is_valid_name(words[1]))
        {
          return false;
        }
        unsigned long long weight = 0;
        if (!detail::parse_ull(words[2], weight))
        {
          return false;
        }
        graph.bind(words[0], words[1], weight);
      }

      if (!storage.add_graph(graph_name, graph))
      {
        return false;
      }
    }
    return true;
  }
}

#endif
