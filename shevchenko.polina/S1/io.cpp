#include "io.hpp"
#include <iostream>
#include <cctype>
#include <limits>
#include <string>

namespace shevchenko
{
size_t getElementAt(const List< size_t >& lst, size_t index, bool& exists)
{
  auto it = lst.cbegin();
  size_t i = 0;

  while (it != lst.cend() && i < index)
  {
    ++it;
    ++i;
  }

  if (it == lst.cend())
  {
    exists = false;
    return 0;
  }

  exists = true;
  return *it;
}

List< Seq > readSequences()
{
  List< Seq > sequences;
  std::string name;
  bool hasName = false;

  while (std::cin >> name)
  {
    hasName = true;
    List< size_t > numbers;

    int ch = std::cin.peek();
    if (ch == '\n')
    {
      std::cin.get();
    }
    else
    {
      while (true)
      {
        size_t number;
        if (!(std::cin >> number))
        {
          if (std::cin.eof())
          {
            std::cin.clear();
          }
          break;
        }
        numbers.pushBack(number);

        ch = std::cin.peek();
        if (ch == '\n')
        {
          std::cin.get();
          break;
        }
        if (ch == EOF)
        {
          break;
        }
      }
    }
    sequences.pushBack({ name, numbers });
  }
  if (!hasName)
  {
    return sequences;
  }
  return sequences;
}

void printNames(const List< Seq >& sequences)
{
  if (sequences.empty())
  {
    return;
  }

  bool first = true;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    if (!first)
    {
      std::cout << ' ';
    }
    std::cout << (*it).first;
    first = false;
  }
  std::cout << '\n';
}

void printTransposed(const List< Seq >& sequences)
{
  if (sequences.empty())
  {
    return;
  }

  size_t maxLen = 0;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t cur = (*it).second.size();
    if (cur > maxLen)
    {
      maxLen = cur;
    }
  }

  if (maxLen == 0)
  {
    return;
  }

  for (size_t i = 0; i < maxLen; ++i)
  {
    bool firstCol = true;
    bool printAny = false;

    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      bool exists = false;
      size_t value = getElementAt((*sit).second, i, exists);

      if (exists)
      {
        if (!firstCol)
        {
          std::cout << ' ';
        }
        std::cout << value;
        firstCol = false;
        printAny = true;
      }
    }

    if (printAny)
    {
      std::cout << '\n';
    }
  }
}

List< size_t > calculateSums(const List< Seq >& sequences)
{
  List< size_t > sums;
  if (sequences.empty())
  {
    return sums;
  }

  size_t maxLen = 0;
  for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
  {
    size_t cur = (*it).second.size();
    if (cur > maxLen)
    {
      maxLen = cur;
    }
  }

  if (maxLen == 0)
  {
    return sums;
  }

  bool overflow = false;
  for (size_t i = 0; i < maxLen; ++i)
  {
    size_t rowSum = 0;
    bool hasAny = false;

    for (auto sit = sequences.cbegin(); sit != sequences.cend(); ++sit)
    {
      bool exists = false;
      size_t value = getElementAt((*sit).second, i, exists);

      if (exists)
      {
        if (rowSum > std::numeric_limits< size_t >::max() - value)
        {
          overflow = true;
        }
        rowSum += value;
        hasAny = true;
      }
    }

    if (hasAny)
    {
      sums.pushBack(rowSum);
    }
  }

  if (overflow)
  {
    throw std::overflow_error("integer overflow");
  }
  return sums;
}

void printSums(const List< size_t >& sums)
{
  if (sums.empty())
  {
    return;
  }
  bool first = true;
  for (auto it = sums.cbegin(); it != sums.cend(); ++it)
  {
    if (!first)
    {
      std::cout << ' ';
    }
    std::cout << *it;
    first = false;
  }
  std::cout << '\n';
}
}
