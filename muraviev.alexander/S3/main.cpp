#include <iostream>

int main(int argc, char* argv[])
{
  if (argc != 2) {
    std::cerr << "filename is required\n";
    return 1;
  }

  (void)argv;
  return 0;
}
