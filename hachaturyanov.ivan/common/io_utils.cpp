#include "io_utils.hpp"

#include <cctype>

namespace hachaturyanov
{
  List< std::string > split(std::string line)
  {
    List< std::string > result;
    std::string substr;
    bool has = false;
    for (size_t i = 0; i < line.size(); i++) {
      char c = line[i];
      if (c == ' ' || c == '\t' || c == '\n') {
        if (has) {
          result.addEnd(substr);
          substr.clear();
          has = false;
        }
      } else {
        substr.push_back(c);
        has = true;
      }
    }
    if (has) {
      result.addEnd(substr);
    }
    return result;
  }

  bool isdigit(std::string str)
  {
    if (str.size() == 0) {
      return false;
    }
    char c = ' ';
    bool digit = true;
    for (size_t i = 0; i < str.size(); i++) {
      c = str[i];
      if (!std::isdigit(c)) {
        digit = false;
      }
    }
    return digit;
  }
}
