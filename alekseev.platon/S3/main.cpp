#include <fstream>
#include <iostream>
#include <string>

#include "command_parser.hpp"
#include "commands.hpp"
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

  alekseev::CommandTable commands = alekseev::make_command_table();
  std::string line;
  while (std::getline(std::cin, line))
  {
    alekseev::Sequence< std::string > args = alekseev::split_words(line);
    try
    {
      alekseev::dispatch_command(commands, args, storage, std::cout);
    }
    catch (const std::exception&)
    {
      alekseev::print_invalid(std::cout);
    }
  }

  return 0;
}
