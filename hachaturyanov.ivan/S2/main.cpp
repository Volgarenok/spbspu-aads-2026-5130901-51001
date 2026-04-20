#include <iostream>
#include <stdexcept>
#include <fstream>
#include "calculator.hpp"

int main(int argc, char* argv[])
{
  try {
    if (argc == 2) {
      std::ifstream input(argv[1]);
      if (!input) {
        std::cerr << "Failed to open file: " << argv[1] << '\n';
        return 1;
      }
      return hachaturyanov::calculate(input);
    } else if (argc == 1) {
      return hachaturyanov::calculate(std::cin);
    } else {
      std::cerr << "Invalid arguments\n";
      return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << e.what() << '\n';
    return 1;
  }
}
