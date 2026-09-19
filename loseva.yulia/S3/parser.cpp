#include "parser.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace loseva {

void loseva::parseGraphFile(const std::string & filename, GraphTable & table)
{
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " + filename);
  }

  std::string line;
  while (std::getline(file, line)) {
    if (line.empty()) {
      continue;
    }

    std::istringstream headerStream(line);
    std::string graphName;
    std::size_t edgeCount = 0;
    if (!(headerStream >> graphName >> edgeCount)) {
      throw std::runtime_error("Invalid graph header: " + line);
    }

    const std::size_t initialCapacity = edgeCount * 2 + 16;
    Graph g(initialCapacity);

    for (std::size_t i = 0; i < edgeCount; ++i) {
      std::string edgeLine;
      do {
        if (!std::getline(file, edgeLine)) {
          throw std::runtime_error(
            "Unexpected end of file while reading edges of " + graphName);
        }
      } while (edgeLine.empty());

      std::istringstream edgeStream(edgeLine);
      std::string from;
      std::string to;
      unsigned int weight = 0;
      if (!(edgeStream >> from >> to >> weight)) {
        throw std::runtime_error("Invalid edge line: " + edgeLine);
      }
      g.addEdge(from, to, weight);
    }

    bool inserted = false;
    while (!inserted) {
      try {
        table.add(graphName, g);
        inserted = true;
      } catch (const TableFullException &) {
        table.rehash(table.capacity() * 2 + 1);
      }
    }
  }
}

}
