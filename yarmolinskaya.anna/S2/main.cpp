#include <fstream>
#include <iostream>
#include <string>

#include "common/list.hpp"
#include "expression.hpp"

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc > 2)
  {
    std::cerr << "invalid arguments\n";
    return 1;
  }

  if (argc == 2)
  {
    file.open(argv[1]);

    if (!file)
    {
      std::cerr << "cannot open file\n";
      return 1;
    }

    input = &file;
  }

  yarmolinskaya::List< long > results;
  std::string line;

  try
  {
    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }

      results.push_front(
        yarmolinskaya::evaluateExpression(line)
      );
    }

    yarmolinskaya::List< long >::Iterator it =
      results.begin();

    while (it != results.end())
    {
      std::cout << *it;

      yarmolinskaya::List< long >::Iterator next = it;
      ++next;

      if (next != results.end())
      {
        std::cout << " ";
      }

      ++it;
    }

    std::cout << "\n";
  }
  catch (const std::exception& error)
  {
    std::cerr << error.what() << "\n";
    return 1;
  }

  return 0;
}
