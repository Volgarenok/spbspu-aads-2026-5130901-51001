#include <iostream>
#include <fstream>

#include "bstree.hpp"
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc != 2) {
    std::cerr << "Incorrect filename\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Unable to open file\n";
    return 1;
  }

  hachaturyanov::Store store;

  hachaturyanov::readInput(store, file);
  hachaturyanov::execute(store, std::cin, std::cout);

  return 0;
}
