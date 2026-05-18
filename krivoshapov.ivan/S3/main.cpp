#include <iostream>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc < 2)
  {
    std::cerr << "ERROR: filename argument required\n";
    return 1;
  }
  std::ifstream file(argv[1]);
  if (!file.is_open())
  {
    std::cerr << "ERROR: cannot open file\n";
    return 1;
  }
  return 0;
}
