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

void yarmolinskaya::processSequences(const List< NamedSequence >& data)
{
  List< List< int >::ConstIterator > iters;
  for (auto it = data.cbegin(); it != data.cend(); ++it)
  {
    iters.push_back((*it).second.cbegin());
  }
  List< int > sums;
  bool any_active = true;
  while (any_active)
  {
    any_active = false;
    List< int > column;
    auto seq_it = data.cbegin();
    auto iter_it = iters.begin();
    while (seq_it != data.cend())
    {
      auto& cur = *iter_it;
      auto end = (*seq_it).second.cend();
      if (cur != end)
      {
        column.push_back(*cur);
        ++cur;
        any_active = true;
      }
      ++seq_it;
      ++iter_it;
    }
    if (any_active)
    {
      int sum = 0;
      bool first = true;
      for (auto it = column.cbegin(); it != column.cend(); ++it)
      {
        if (!first)
        {
          std::cout << " ";
        }
        std::cout << *it;
        sum += *it;
        first = false;
      }
      std::cout << "\n";
      sums.push_back(sum);
    }
  }
  bool first = true;
  for (auto it = sums.cbegin(); it != sums.cend(); ++it)
  {
    if (!first)
    {
      std::cout << " ";
    }
    std::cout << *it;
    first = false;
  }
  std::cout << "\n";
}
