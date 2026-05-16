#include "parsing.hpp"

std::vector< std::string > muraviev::splitTokens(const std::string& line)
{
  std::vector< std::string > tokens;
  std::string token;
  for (size_t i = 0; i < line.size(); ++i) {
    if (line[i] == ' ') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else {
      token += line[i];
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }
  return tokens;
}
