#include "parsing.hpp"

bool muraviev::splitStrictSpaces(const std::string& line,
    std::vector< std::string >& tokens)
{
  tokens.clear();
  if (line.empty()) {
    return true;
  }
  if (line[0] == ' ' || line[line.size() - 1] == ' ') {
    return false;
  }

  std::string token;
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == ' ') {
      if (token.empty()) {
        return false;
      }
      tokens.push_back(token);
      token.clear();
    } else {
      token += line[i];
    }
  }
  tokens.push_back(token);
  return true;
}
