#include <iostream>

#include "list.hpp"

int main()
{
  loseva::List<int> list;

  list.pushBack(1);
  list.pushBack(2);
  list.pushFront(0);

  std::cout << list.front() << std::endl;
  std::cout << list.back() << std::endl;
  std::cout << list.size() << std::endl;

  return 0;
}
