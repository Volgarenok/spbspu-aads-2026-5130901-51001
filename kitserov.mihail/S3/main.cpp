#include <iostream>
#include <fstream>
#include "hash.hpp"
#include "graph.hpp"

int main(int argc, char* argv[])
{
  using namespace kitserov;
  using CommandHandler = void (*)(std::ostream& out, std::istream& in);
  if (argc != 2) {
    std::cerr << "Must be 2 argc\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error: cannot open file \"" << argv[1] << "\"" << std::endl;
    return 1;
  }
  std::vector< std::string, Graph > graphs;
  std::string word;
  size_t countEdges = 0;
  while (inputFile >> word) {
    if (countEdges == 0) {
      Graph g(word);
      if (!(inputFile >> countEdges)) {
        std::cerr << "Error: expected edge count after graph name\n";
        return 2;
      }
      graphs.add(word, std::move(g));
    } else {
      Graph& g = *(graphs.begin());
      std::string src = word;
      std::string dst;
      size_t weight = 0;
      if (!(inputFile >> dst >> weight)) {
        std::cerr << "Error: incomplete edge data after vertex \n";
        return 2;
      }
      g.addEdge(src, dst, weight);
      --countEdges;
    }
  }

  HashTable< std::string, CommandHandler, SipHash< std::string > > cmds(20);
  
  cmds.add("graphs", cmd_graphs);
  cmds.add("vertexes", cmd_vertexes);
  cmds.add("outbound", cmd_outbound);
  cmds.add("inbound", cmd_inbound);
  cmds.add("bind", cmd_bind);
  cmds.add("cut", cmd_cut);
  cmds.add("create", cmd_create);
  cmds.add("merge", cmd_merge);
  cmds.add("extract", cmd_extract);

  std::string cmd;
  while (std::cin >> cmd)
  {
    try {
      auto handler = cmds.find(cmd);
      if (!handler) {
        throw std::invalid_argument("unknown command");
      }
      (*handler)(std::cout, std::cin);
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } catch (...) {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }
  return 0;
}