#include "io.hpp"
#include <iostream>
#include <cctype>

namespace shevchenko
{
List< Seq > readSequences()
{
  List< Seq > sequences;
  std::string name;
  
  while (std::cin >> name)
  {
    List< int > numbers;
    int value = 0;
    
    while (std::cin.peek() != EOF && !std::isalpha(std::cin.peek()))
    {
      if (std::cin >> value)
      {
        numbers.pushBack(value);
      }
      else
      {
        std::cin.clear();
        break;
      }
      while (std::cin.peek() == ' ')
      {
        std::cin.get();
      }
    }
    sequences.pushBack({ name, numbers });
  }
  
  return sequences;
}

void printNames(const List< Seq >& sequences)
{
  bool first = true;

  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
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

void printTransposed(const List< Seq >& sequences)
{
  size_t max_len = 0;

  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t s = (*it).second.size();
    if (s > max_len)
    {
      max_len = s;
    }
  }

  for (size_t i = 0; i < max_len; ++i)
  {
    bool first = true;

    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      if (!first)
      {
        std::cout << " ";
      }

      auto nit = (*sit).second.cbegin();
      for (size_t j = 0; j < i && nit != (*sit).second.cend(); ++j)
      {
        ++nit;
      }

      if (nit != (*sit).second.cend())
      {
        std::cout << *nit;
      }
      first = false;
    }
    std::cout << "\n";
  }
}

List< int > calculateSums(const List< Seq >& sequences)
{
  List< int > sums;
  size_t max_len = 0;

  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t s = (*it).second.size();
    if (s > max_len)
    {
      max_len = s;
    }
  }

  for (size_t i = 0; i < max_len; ++i)
  {
    int sum = 0;
    bool has_number = false;

    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      auto nit = (*sit).second.cbegin();
      for (size_t j = 0; j < i && nit != (*sit).second.cend(); ++j)
      {
        ++nit;
      }

      if (nit != (*sit).second.cend())
      {
        sum += *nit;
        has_number = true;
      }
    }

    if (has_number)
    {
      sums.pushBack(sum);
    }
  }

  return sums;
}

void printSums(const List< int >& sums)
{
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
}
