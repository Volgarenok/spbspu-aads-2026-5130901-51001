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
  std::stringstream& ss
)
{
  std::string extra;

  return static_cast< bool >(ss >> extra);
}

namespace
{
  using yarmolinskaya::Graph;
  using yarmolinskaya::GraphTable;

  void graphsCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    if (yarmolinskaya::hasExtraArguments(ss))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    std::vector< std::string > names;

    for (
      GraphTable::const_iterator it =
        graphs.begin();
      it != graphs.end();
      ++it
    )
    {
      names.push_back(it->first);
    }

    std::sort(
      names.begin(),
      names.end()
    );

    for (const std::string& name : names)
    {
      std::cout << name << '\n';
    }
  }

  void vertexesCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;

    if (!(ss >> graphName))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
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

    for (const std::string& vertex : vertexes)
    {
      std::cout << vertex << '\n';
    }
  }

  void outboundCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;
    std::string vertex;

    if (!(ss >> graphName >> vertex))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
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

    Graph::AdjacencyList result =
      graphs.at(graphName).outbound(vertex);

    for (const auto& item : result)
    {
      std::cout << item.first;

      for (size_t weight : item.second)
      {
        std::cout << ' ' << weight;
      }

      std::cout << '\n';
    }
  }

  void inboundCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;
    std::string vertex;

    if (!(ss >> graphName >> vertex))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
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

    Graph::AdjacencyList result =
      graphs.at(graphName).inbound(vertex);

    for (const auto& item : result)
    {
      std::cout << item.first;

      for (size_t weight : item.second)
      {
        std::cout << ' ' << weight;
      }

      std::cout << '\n';
    }
  }

  void bindCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;
    std::string from;
    std::string to;

    size_t weight = 0;

    if (
      !(ss >> graphName >>
        from >>
        to >>
        weight)
    )
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
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

    graphs.at(graphName).bind(
      from,
      to,
      weight
    );
  }

  void cutCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;
    std::string from;
    std::string to;

    size_t weight = 0;

    if (
      !(ss >> graphName >>
        from >>
        to >>
        weight)
    )
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (
      !graphs.contains(graphName) ||
      !graphs.at(graphName).cut(
        from,
        to,
        weight
      )
    )
    {
      throw std::logic_error(
        "invalid cut"
      );
    }
  }

  void createCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string graphName;

    size_t count = 0;

    if (!(ss >> graphName >> count))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (graphs.contains(graphName))
    {
      throw std::logic_error(
        "graph already exists"
      );
    }

    Graph graph;

    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;

      if (!(ss >> vertex))
      {
        throw std::logic_error(
          "invalid vertex count"
        );
      }

      if (graph.hasVertex(vertex))
      {
        throw std::logic_error(
          "duplicate vertex"
        );
      }

      graph.addVertex(vertex);
    }

    if (
      yarmolinskaya::hasExtraArguments(ss)
    )
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    graphs.insert(graphName, graph);
  }

  void mergeCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string newGraph;
    std::string lhs;
    std::string rhs;

    if (
      !(ss >> newGraph >>
        lhs >>
        rhs)
    )
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (yarmolinskaya::hasExtraArguments(ss))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (
      graphs.contains(newGraph) ||
      !graphs.contains(lhs) ||
      !graphs.contains(rhs)
    )
    {
      throw std::logic_error(
        "invalid merge"
      );
    }

    graphs.insert(
      newGraph,
      Graph::merge(
        graphs.at(lhs),
        graphs.at(rhs)
      )
    );
  }

  void extractCommand(
    std::stringstream& ss,
    GraphTable& graphs
  )
  {
    std::string newGraph;
    std::string source;

    size_t count = 0;

    if (
      !(ss >> newGraph >>
        source >>
        count)
    )
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    if (
      graphs.contains(newGraph) ||
      !graphs.contains(source)
    )
    {
      throw std::logic_error(
        "invalid extract"
      );
    }

    std::vector< std::string > vertexes;

    for (size_t i = 0; i < count; ++i)
    {
      std::string vertex;

      if (!(ss >> vertex))
      {
        throw std::logic_error(
          "invalid vertex count"
        );
      }

      vertexes.push_back(vertex);
    }

    if (yarmolinskaya::hasExtraArguments(ss))
    {
      throw std::logic_error(
        "invalid arguments"
      );
    }

    graphs.insert(
      newGraph,
      Graph::extract(
        graphs.at(source),
        vertexes
      )
    );
  }
}

yarmolinskaya::CommandTable
yarmolinskaya::createCommandTable()
{
  CommandTable commands(23, 2, 23);

  commands.insert("graphs", graphsCommand);
  commands.insert("vertexes", vertexesCommand);
  commands.insert("outbound", outboundCommand);
  commands.insert("inbound", inboundCommand);
  commands.insert("bind", bindCommand);
  commands.insert("cut", cutCommand);
  commands.insert("create", createCommand);
  commands.insert("merge", mergeCommand);
  commands.insert("extract", extractCommand);

  return commands;
}
