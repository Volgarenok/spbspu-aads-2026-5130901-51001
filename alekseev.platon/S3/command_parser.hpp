#ifndef ALEKSEEV_S3_COMMAND_PARSER_HPP
#define ALEKSEEV_S3_COMMAND_PARSER_HPP

#include <string>

#include "input_reader.hpp"
#include "sequence.hpp"

namespace alekseev
{
  inline Sequence< std::string > split_words(const std::string& line)
  {
    Sequence< std::string > words;
    size_t pos = 0;
    while (pos < line.size())
    {
      while (pos < line.size() && line[pos] == ' ')
      {
        ++pos;
      }
      if (pos == line.size())
      {
        break;
      }
      const size_t start = pos;
      while (pos < line.size() && line[pos] != ' ')
      {
        ++pos;
      }
      words.push_back(line.substr(start, pos - start));
    }
    return words;
  }

  inline bool parse_size(const std::string& value, size_t& result)
  {
    return detail::parse_size(value, result);
  }

  inline bool parse_ull(const std::string& value, unsigned long long& result)
  {
    return detail::parse_ull(value, result);
  }
}

#endif
