#include <fstream>
#include <iostream>

namespace alekseev
{
  bool load_graphs(std::istream&)
  {
    return true;
  }
}

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

  if (!alekseev::load_graphs(input))
  {
    return 1;
  }

  return 0;
}
