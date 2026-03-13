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
