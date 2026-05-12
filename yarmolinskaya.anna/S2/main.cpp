#include <fstream>
#include <iostream>
#include <string>

#include "common/list.hpp"
#include "expression.hpp"

int main(int argc, char* argv[])
{
  using namespace yarmolinskaya;

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

  List< long > results;
  std::string line;

  try
  {
    while (std::getline(*input, line))
    {
      if (line.empty())
      {
        continue;
      }

      results.push_front(evaluateExpression(line));
    }

    for (auto it = results.begin(); it != results.end(); ++it)
    {
      std::cout << *it;

      auto next = it;
      ++next;

      if (next != results.end())
      {
        std::cout << " ";
      }
    }

    std::cout << "\n";
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }

  return 0;
}
