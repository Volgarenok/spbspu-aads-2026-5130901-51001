#include "commands.hpp"
#include "io.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr <<
      "invalid arguments\n";

    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr <<
      "cannot open file\n";

    return 1;
  }

  yarmolinskaya::GraphTable graphs(
    53,
    2,
    53
  );

  try
  {
    yarmolinskaya::readGraphs(
      input,
      graphs
    );
  }
  catch (const std::logic_error& error)
  {
    std::cerr
      << error.what()
      << '\n';

    return 1;
  }

  yarmolinskaya::CommandTable commands =
    yarmolinskaya::createCommandTable();

  std::string line;

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::stringstream inputStream(line);

    std::string command;

    inputStream >> command;

    try
    {
      if (!commands.contains(command))
      {
        throw std::logic_error(
          "unknown command"
        );
      }

      commands.at(command)(
        inputStream,
        graphs
      );
    }
    catch (const std::logic_error&)
    {
      yarmolinskaya::printInvalidCommand();
    }
  }

  return 0;
}
