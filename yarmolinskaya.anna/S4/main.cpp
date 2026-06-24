#include <fstream>
#include <iostream>
#include <string>

#include "commands.hpp"

int main(int argc, char * argv[])
{
  using namespace yarmolinskaya;

  if (argc != 2) {
    std::cerr << "Wrong arguments\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input) {
    std::cerr << "Cannot open file\n";
    return 1;
  }

  Dictionaries data;

  readDictionaries(input, data);

  std::string command;

  while (std::cin >> command) {
    try {
      if (command == "print") {
        std::string name;
        std::cin >> name;

        printDictionary(data, name, std::cout);
      } else if (command == "complement") {
        std::string newName;
        std::string left;
        std::string right;

        std::cin >> newName >> left >> right;

        makeComplement(data, newName, left, right);
      } else if (command == "intersect") {
        std::string newName;
        std::string left;
        std::string right;

        std::cin >> newName >> left >> right;

        makeIntersect(data, newName, left, right);
      } else if (command == "union") {
        std::string newName;
        std::string left;
        std::string right;

        std::cin >> newName >> left >> right;

        makeUnion(data, newName, left, right);
      } else {
        std::cout << "<INVALID COMMAND>\n";
      }
    } catch (const std::exception &) {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}
