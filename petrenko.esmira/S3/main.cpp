#include "hash-table.hpp"
#include "graph.hpp"
#include "sha1.hpp"
#include <iostream>

namespace petrenko {

HashTable<std::string, Graph*, std::hash<std::string>, std::equal_to<std::string> > graphs;

}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " filename" << std::endl;
    return 1;
  }

  return 0;
}
