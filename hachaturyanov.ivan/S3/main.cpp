#include <fstream>
#include "commands.hpp"

int main(int argc, char** argv)
{
  if (argc < 2) {
    std::cerr << "Filename required" << '\n';
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open()) {
    std::cerr << "Cannot open file" << '\n';
    return 1;
  }

  hachaturyanov::GraphTable graphs;

  hachaturyanov::readFile(file, graphs);
  hachaturyanov::process(std::cin, std::cout, graphs);

  return 0;
}
