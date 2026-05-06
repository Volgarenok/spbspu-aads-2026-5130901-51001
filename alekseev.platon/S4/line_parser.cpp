#include "line_parser.hpp"

#include <cctype>

namespace alekseev
{
  LineParser::LineParser(const std::string& line):
    line_(line),
    pos_(0)
  {
  }

  bool LineParser::hasNext() const
  {
    size_t pos = pos_;
    while (pos < line_.size() && std::isspace(static_cast< unsigned char >(line_[pos])))
    {
      ++pos;
    }
    return pos < line_.size();
  }
}
