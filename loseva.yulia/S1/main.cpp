#include <iostream>

#include "list.hpp"

int main()
{
  loseva::List<int> list;

  list.pushBack(10);
  list.pushBack(20);
  list.pushFront(5);

  std::cout << list.front() << std::endl;
  std::cout << list.back() << std::endl;
  std::cout << list.size() << std::endl;

  return 0;
}
