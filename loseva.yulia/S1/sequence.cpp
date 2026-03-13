#include <iostream>
#include "sequence.hpp"

namespace loseva
{

void read_sequences(Storage& data)
{
  std::string name;

  while (std::cin >> name)
  {
    Seq seq;

    int value;

    while (std::cin.peek() != '\n' && std::cin >> value)
    {
      seq.push_back(value);
    }

    data.push_back({name, seq});
  }
}

void print_names(const Storage& data)
{
  for (auto it = data.begin(); it != data.end(); ++it)
  {
    std::cout << (*it).first << " ";
  }

  std::cout << "\n";
}

}
