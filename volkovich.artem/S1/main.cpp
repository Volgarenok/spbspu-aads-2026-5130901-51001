#include <iostream>
#include "list.hpp"
#include <string>
#include <utility>
#include <limits>

typedef volkovich::List<std::pair<std::string, volkovich::List<size_t>>> DATA_TYPE;
typedef volkovich::LIter<std::pair<std::string, volkovich::List<size_t>>> ITER_TYPE;

size_t getAt(volkovich::List<size_t> &lst, size_t colNum, bool &isLast)
{
  volkovich::LIter<size_t> it = lst.begin();
  size_t i = 0;
  while (it != volkovich::LIter<size_t>(nullptr) && i < colNum)
  {
    ++it;
    ++i;
  }
  if (it == volkovich::LIter<size_t>(nullptr))
  {
    isLast = true;
    return 0;
  }
  isLast = false;
  return *it;
}

int main()
{
  DATA_TYPE data;
  bool hasNumbers = false;
  bool hasName = false;
  std::string name;
  while (std::cin >> name)
  {
    hasName = true;
    volkovich::List<size_t> numbers;
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
        hasNumbers = true;

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
    data.pushBack(std::make_pair(name, numbers));
  }
  if(!hasName) {
    std::cout<<'\n';
    return 0;
  }
  bool first = true;
  for (ITER_TYPE it = data.begin();
       it != ITER_TYPE(nullptr); ++it)
  {
    if (!first)
    {
      std::cout << ' ';
    }
    std::cout << (*it).first;
    first = false;
  }
  std::cout << '\n';
  if (!hasNumbers)
  {
    std::cout << "0\n";
    return 0;
  }

  size_t maxLen = 0;
  for (ITER_TYPE it = data.begin();
       it != ITER_TYPE(nullptr); ++it)
  {
    size_t cur = (*it).second.size();
    if (cur > maxLen)
    {
      maxLen = cur;
    }
  }

  volkovich::List<size_t> rowSumsList;
  bool overflow = false;

  for (size_t i = 0; i < maxLen; ++i)
  {
    bool firstCol = true;
    size_t rowSum = 0;
    for (ITER_TYPE it = data.begin();
         it != ITER_TYPE(nullptr); ++it)
    {
      bool isLast = true;
      size_t value = getAt((*it).second, i, isLast);
      if (!isLast)
      {
        if (!firstCol)
        {
          std::cout << ' ';
        }
        if (rowSum > std::numeric_limits<size_t>::max() - value)
        {
          overflow = true;
        }
        rowSum += value;
        std::cout << value;
        firstCol = false;
      }
    }
    rowSumsList.pushBack(rowSum);
    std::cout << '\n';
  }

  if (overflow)
  {
    return 1;
  }

  bool firstSum = true;
  for (volkovich::LIter<size_t> it = rowSumsList.begin();
       it != volkovich::LIter<size_t>(nullptr); ++it)
  {
    if (!firstSum)
    {
      std::cout << ' ';
    }
    std::cout << *it;
    firstSum = false;
  }
  std::cout << '\n';

  return 0;
}
