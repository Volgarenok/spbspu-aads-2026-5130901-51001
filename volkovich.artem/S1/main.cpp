#include <iostream>
#include "list.hpp"
#include <string>
#include <utility>

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
  std::string name;
  while (std::cin >> name)
  {
    volkovich::List<size_t> numbers;
    size_t number;
    while (true)
    {
      std::cin >> number;
      if (std::cin.fail())
      {
        std::cin.clear();
        char c;
        if (std::cin.get(c) && c == '\n')
          break;
      }
      numbers.pushBack(number);
      if (std::cin.peek() == '\n')
      {
        std::cin.get();
        break;
      }
    }
    data.pushBack(std::make_pair(name, numbers));
  }

  if (data.isEmpty())
  {
    std::cout << "0\n";
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

  for (size_t i = 0; i < maxLen; ++i)
  {
    bool firstCol = true;
    for (ITER_TYPE it = data.begin();
         it != ITER_TYPE(nullptr); ++it)
    {
      if (!firstCol)
      {
        std::cout << ' ';
      }
      bool isLast = true;
      size_t value = getAt((*it).second, i, isLast);
      if (!isLast)
      {
        std::cout << value;
      }
      firstCol = false;
    }
    std::cout << '\n';
  }

  return 0;
}

