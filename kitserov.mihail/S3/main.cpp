#include <map>
#include <iostream>
#include "hash.hpp"

int main(int argc, char* argv[])
{
  using namespace kitserov;
  if (argc != 2) {
    std::cerr << "Must be 2 argc\n";
    return 1;
  }
  std::map< std::string, void(*)(std::ostream&, std::istream&) > cmds;
  cmds["blank"] = blank;

  std::string cmd;
  while (std::cin >> cmd)
  {
    try {
      cmds.at(cmd)(std::cout, std::cin);
    } catch (...) {
      std::cerr << "Error\n";
      return 2;
    }
  }
  return 0;
}