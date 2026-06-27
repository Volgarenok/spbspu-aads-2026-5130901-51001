#include <iostream>
#include <fstream>
#include <string>

#include "expression.hpp"
#include "evaluator.hpp"
#include "stack.hpp"

int main(int argc, char* argv[])
{
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc > 1)
  {
    file.open(argv[1]);
    if (!file)
    {
      std::cerr << "Cannot open file\n";
      return 1;
    }
    input = &file;
  }

  smirnova::Stack<long long> results;
  std::string line;

  try
  {
    while (std::getline(*input, line))
    {
      while (!line.empty() && (line.back() == '\r' || line.back() == ' '))
        line.pop_back();

      if (line.empty())
          continue;

      auto postfix = smirnova::toPostfix(line);
      long long res = smirnova::evaluatePostfix(postfix);

      results.push(res);
    }

    while (!results.empty())
    {
      std::cout << results.drop();
      if (!results.empty())
        std::cout << " ";
    }

    std::cout << std::endl;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

