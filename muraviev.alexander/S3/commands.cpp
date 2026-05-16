#include "commands.hpp"

#include <fstream>
#include <vector>

#include "parsing.hpp"

bool muraviev::loadGraphs(const std::string& filename, GraphTable& graphs)
{
  std::ifstream input(filename.c_str());
  if (!input) {
    return false;
  }

  std::string line;
  while (std::getline(input, line)) {
    if (line.empty()) {
      continue;
    }
    std::vector< std::string > tokens;
    if (!splitStrictSpaces(line, tokens) || tokens.size() != 2 || !isValidName(tokens[0])) {
      return false;
    }

    size_t count = 0;
    if (!parseSize(tokens[1], count)) {
      return false;
    }
    Graph graph(tokens[0]);
    for (size_t i = 0; i < count; ++i) {
      std::getline(input, line);
      std::vector< std::string > edge;
      unsigned long long weight = 0;
      if (!splitStrictSpaces(line, edge) || edge.size() != 3 ||
          !parseUnsignedLongLong(edge[2], weight)) {
        return false;
      }
      graph.addEdge(edge[0], edge[1], weight);
    }
    graphs.add(tokens[0], graph);
  }
  return true;
}
