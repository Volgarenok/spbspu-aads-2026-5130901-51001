#include <iostream>
#include <fstream>
#include <string>

#include "graph.hpp"
#include "hash_table.hpp"
#include "commands.hpp"
#include "sha1.hpp"

int main(int argc, char* argv[])
{
  using namespace shevchenko;
  
  try
  {
    if (argc != 2)
    {
      std::cerr << "error: filename required\n";
      return 1;
    }
    
    GraphTable graphs;

    {
      std::ifstream file(argv[1]);
      if (!file)
      {
        std::cerr << "error: cannot open file\n";
        return 1;
      }
      
      std::string graph_name;
      size_t edge_count;
      
      while (file >> graph_name >> edge_count)
      {
        Graph g;
        
        for (size_t i = 0; i < edge_count; ++i)
        {
          std::string from, to;
          size_t weight;
          
          file >> from >> to >> weight;
          g.bind(from, to, weight);
        }
        
        graphs.insert(graph_name, g);
      }
    }

    std::string cmd;
    
    while (std::cin >> cmd)
    {
      try
      {
        if (cmd == "graphs")
        {
          graphsCommand(graphs, std::cout);
        }
        else if (cmd == "vertexes")
        {
          vertexesCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "outbound")
        {
          outboundCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "inbound")
        {
          inboundCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "bind")
        {
          bindCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "cut")
        {
          cutCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "create")
        {
          createCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "merge")
        {
          mergeCommand(graphs, std::cin, std::cout);
        }
        else if (cmd == "extract")
        {
          extractCommand(graphs, std::cin, std::cout);
        }
        else
        {
          printInvalid(std::cout);
        }
      }
      catch (const std::exception&)
      {
        printInvalid(std::cout);
      }
    }
  }
  catch (const std::exception& e)
  {
    std::cerr << "error: " << e.what() << '\n';
    return 1;
  }
  
  return 0;
}
