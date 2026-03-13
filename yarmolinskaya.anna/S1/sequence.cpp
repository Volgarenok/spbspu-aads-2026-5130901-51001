#include "sequence.hpp"

#include <iostream>

void yarmolinskaya::readSequences(List< NamedSequence >& data)
{
  std::string name;

  while (std::cin >> name)
  {
    List< int > nums;
    int value;

    while (std::cin >> value)
    {
      nums.push_back(value);
    }

    std::cin.clear();

    data.push_back(NamedSequence(name, std::move(nums)));
  }
}

void yarmolinskaya::printNames(const List< NamedSequence >& data)
{
  bool first = true;

  for (auto it = data.cbegin(); it != data.cend(); ++it)
  {
    if (!first)
    {
      std::cout << " ";
    }

    std::cout << (*it).first;
    first = false;
  }

  std::cout << "\n";
}
