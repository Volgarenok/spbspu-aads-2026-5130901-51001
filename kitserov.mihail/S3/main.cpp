#include <iostream>
#include <fstream>
#include "hash.hpp"
#include "graph.hpp"

int main(int argc, char* argv[])
{
  using namespace kitserov;
  if (argc != 2) {
    std::cerr << "Must be 2 argc\n";
    return 1;
  }
  std::ifstream inputFile(argv[1]);
  if (!inputFile) {
    std::cerr << "Error: cannot open file \"" << argv[1] << "\"" << std::endl;
    return 1;
  }
  std::vector< Graph > graphs;
  std::string word;
  size_t countEdges = 0;
  while (inputFile >> word) {
    if (countEdges == 0) {
      Graph g(word);
      if (!(inputFile >> countEdges)) {
        std::cerr << "Error: expected edge count after graph name\n";
        return 2;
      }
      graphs.push_back(std::move(g));
    } else {
      Graph& g = graphs.back();
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

  HashTable< std::string, void(*)(std::ostream&, std::istream&), SipHash< std::string >, std::equal_to< std::string > > cmds(20);
  
  cmds.add("graphs", graphs);
  cmds.add("vertexes", vertexes);
  cmds.add("outbound", outbound);
  cmds.add("inbound", inbound);
  cmds.add("", );  

  std::string cmd;
  while (std::cin >> cmd)
  {
    try {
      cmds.at(cmd)(std::cout, std::cin);
    } catch (...) {
      std::cerr << "Error\n";
      return 2;
    }
  }
  return 0;
}