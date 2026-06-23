#include "commands.hpp"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

void yarmolinskaya::printInvalidCommand()
{
  std::cout << "<INVALID COMMAND>\n";
}

bool yarmolinskaya::hasExtraArguments(
  std::istream& input
)
{
  std::string extra;

  return static_cast< bool >(
    input >> extra
  );
}

namespace
{
  using yarmolinskaya::GraphTable;

  void graphsCommand(
    std::istream&,
    GraphTable& graphs
  )
  {
    std::vector< std::string > names;

    for (
      GraphTable::const_iterator it =
        graphs.begin();
      it != graphs.end();
      ++it
    )
    {
      names.push_back(
        it->first
      );
    }

    std::sort(
      names.begin(),
      names.end()
    );

    for (
      const std::string& name :
      names
    )
    {
      std::cout
        << name
        << '\n';
    }
  }

  void vertexesCommand(
    std::istream& input,
    GraphTable& graphs
  )
  {
    std::string graphName;

    if (!(input >> graphName))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (hasExtraArguments(input))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (!graphs.contains(graphName))
    {
      throw std::logic_error(
        "graph not found"
      );
    }

    std::vector< std::string > vertexes =
      graphs.at(graphName).vertexes();

    for (
      const std::string& vertex :
      vertexes
    )
    {
      std::cout
        << vertex
        << '\n';
    }
  }
}

yarmolinskaya::CommandTable
yarmolinskaya::createCommandTable()
{
  CommandTable commands(
    23,
    2,
    23
  );

  commands.insert(
    "graphs",
    graphsCommand
  );

  commands.insert(
    "vertexes",
    vertexesCommand
  );

  return commands;
}
