#include <fstream>
#include <iostream>
#include <string>
#include "commands.hpp"
#include "graph.hpp"
#include "hashTable.hpp"

int main(int argc, char* argv[])
{
  namespace sm = smirnova;

  if (argc != 2)
  {
    std::cerr << "Wrong arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  sm::GraphTable graphs;
  sm::VertTable graphVertices;

  std::string graphName;
  size_t edgeCount = 0;

  while (input >> graphName >> edgeCount)
  {
    if (!graphs.has(graphName))
    {
      graphs.add(graphName, sm::Graph());
      graphVertices.add(graphName, sm::Vector<std::string>());
    }

    for (size_t i = 0; i < edgeCount; ++i)
    {
      std::string a, b;
      int w = 0;
      if (!(input >> a >> b >> w))
        return 1;

      sm::Vector<std::string>& verts = graphVertices.get(graphName);
      bool hasA = false, hasB = false;
      for (size_t j = 0; j < verts.size(); ++j)
      {
        if (verts[j] == a) hasA = true;
        if (verts[j] == b) hasB = true;
      }
      if (!hasA) verts.pushBack(a);
      if (!hasB) verts.pushBack(b);

      graphs.get(graphName).addEdge(a, b, w);
    }
  }

  using cmd_t = void (*)(std::istream&, std::ostream&, sm::GraphTable&, sm::VertTable&, std::string);
  sm::HashTable<std::string, cmd_t> cmds;

  cmds.add("create", sm::create);
  cmds.add("bind", sm::bind);
  cmds.add("cut", sm::cut);
  cmds.add("outbound", sm::outbound);
  cmds.add("inbound", sm::inbound);
  cmds.add("merge", sm::merge);
  cmds.add("extract", sm::extract);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try
    {
      if (cmd == "graphs")
      {
        sm::graphs(std::cin, std::cout, graphs, graphVertices, "");
        continue;
      }

      if (cmd == "vertexes")
      {
        std::string graphNameCmd;
        if (!(std::cin >> graphNameCmd))
        {
          std::cout << "<INVALID COMMAND>\n";
          break;
        }
        sm::vertexes(std::cin, std::cout, graphs, graphVertices, graphNameCmd);
        continue;
      }

      if (!cmds.has(cmd))
      {
        std::string graphNameCmd;
        if (!(std::cin >> graphNameCmd))
        {
          std::cout << "<INVALID COMMAND>\n";
          break;
        }
        std::cout << "<INVALID COMMAND>\n";
        continue;
      }

      std::string graphNameCmd;
      if (!(std::cin >> graphNameCmd))
      {
        std::cout << "<INVALID COMMAND>\n";
        break;
      }
      cmds.get(cmd)(std::cin, std::cout, graphs, graphVertices, graphNameCmd);
    }
    catch (const std::out_of_range&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
    catch (const std::logic_error&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}


