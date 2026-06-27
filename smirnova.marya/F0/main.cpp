#include <iostream>
#include <exception>
#include "commands.hpp"

int main()
{
  try
  {
    smirnova::executeCommands(std::cin, std::cout);
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
  return 0;
}

