#include "commands.hpp"
#include "parser.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char * argv[])
{
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>\n";
    return 1;
  }

  const std::string filename(argv[1]);

  lab::GraphTable table(64);
  try {
    lab::parseGraphFile(filename, table);
  } catch (const std::exception & e) {
    std::cerr << "Error reading file: " << e.what() << "\n";
    return 1;
  }

  try {
    lab::runCommands(table, std::cin, std::cout);
  } catch (const std::exception & e) {
    std::cerr << "Internal error: " << e.what() << "\n";
    return 2;
  }

  return 0;
}
