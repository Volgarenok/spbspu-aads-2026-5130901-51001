#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../common/bstree.hpp"
#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "ERROR: filename required\n";
    return 1;
  }

  std::ifstream input(argv[1]);

  if (!input)
  {
    std::cerr << "ERROR: cannot open file\n";
    return 1;
  }

  using Dict = smirnova::BSTree< int, std::string, smirnova::Less< int > >;
  smirnova::BSTree< std::string, Dict, smirnova::Less<std::string > > datasets;

  std::string line;

  while (std::getline(input, line))
  {
    if (line.empty())
    {
      continue;
    }

    std::istringstream iss(line);

    std::string name;
    iss >> name;

    Dict dict;

    int key;
    std::string value;

    while (iss >> key >> value)
    {
      dict.push(key, value);
    }

    datasets.push(name, dict);
  }

  while (std::getline(std::cin, line))
  {
    if (line.empty())
    {
      continue;
    }

    try
    {
      std::istringstream iss(line);
      smirnova::executeCommand(datasets, iss);
    }
    catch (...)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }

  return 0;
}

