#include <iostream>
#include "sequence.hpp"

int main()
{
  loseva::Storage data;

  loseva::read_sequences(data);

  if (data.empty())
  {
    std::cout << 0;
    return 0;
  }

  loseva::print_names(data);

  return 0;
}
