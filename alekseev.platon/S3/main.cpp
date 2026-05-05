#include <fstream>
#include <iostream>

#include "graph_storage.hpp"
#include "input_reader.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "filename is required\n";
    return 1;
  }

  std::ifstream input(argv[1]);
  if (!input)
  {
    std::cerr << "cannot open file\n";
    return 1;
  }

  alekseev::GraphStorage storage;
  if (!alekseev::load_graphs(input, storage))
  {
    return 1;
  }

  return 0;
}
