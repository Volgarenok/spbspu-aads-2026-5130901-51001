#include "io.hpp"

#include <stdexcept>
#include <string>

void yarmolinskaya::readGraphs(
  std::ifstream& input,
  GraphTable& graphs
)
{
  std::string graphName;

  while (input >> graphName)
  {
    size_t edgeCount = 0;

    if (!(input >> edgeCount))
    {
      throw std::logic_error(
        "invalid input file"
      );
    }

    Graph graph;

    for (size_t i = 0; i < edgeCount; ++i)
    {
      std::string from;
      std::string to;

      size_t weight = 0;

      if (
        !(input >>
          from >>
          to >>
          weight)
      )
      {
        throw std::logic_error(
          "invalid input file"
        );
      }

      graph.bind(
        from,
        to,
        weight
      );
    }

    if (!graphs.insert(graphName, graph))
    {
      throw std::logic_error(
        "duplicate graph"
      );
    }
  }
}
