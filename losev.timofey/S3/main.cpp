#include <iostream>

int main(int argc, char* argv[])
{
  (void)argv;
  
  if (argc != 2) {
    std::cerr << "Error: filename required\n";
    return 1;
  }
  return 0;
}
