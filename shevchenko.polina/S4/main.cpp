#include <fstream>
#include <iostream>
#include <limits>

#include "commands.hpp"

int main(int argc, char* argv[])
{
  if (argc != 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "cannot open file\n";
    return 1;
  }

  shevchenko::Dictionaries dicts;
  shevchenko::readDict(file, dicts);

  std::string command;
  while (std::cin >> command)
  {
    if (command == "print")
    {
      shevchenko::doPrint(dicts, std::cin, std::cout);
    }
    else if (command == "union")
    {
      shevchenko::doUnion(dicts, std::cin, std::cout);
    }
    else if (command == "intersect")
    {
      shevchenko::doIntersect(dicts, std::cin, std::cout);
    }
    else if (command == "complement")
    {
      shevchenko::doComplement(dicts, std::cin, std::cout);
    }
    else
    {
      std::cout << "<INVALID COMMAND>\n";
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
  }

  return 0;
}
