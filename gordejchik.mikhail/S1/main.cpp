#include <iostream>
#include <string>
#include "list.hpp"
#include "named-list.hpp"

namespace gordejchik {
  static void readInput(List< NamedList >& namedLists)
  {
    std::string name;
    while (std::cin >> name) {
      NamedList namedList(name);
      if (std::cin.eof()) {
        namedLists.pushBack(static_cast< NamedList&& >(namedList));
        break;
      }
      size_t num = 0;
      while (std::cin >> num) {
        namedList.nums.pushBack(num);
      }
      namedLists.pushBack(static_cast< NamedList&& >(namedList));
      std::cin.clear();
    }
  }

  static void printNames(const List< NamedList >& namedLists)
  {
    bool first = true;
    for (auto it = namedLists.cbegin(); it != namedLists.cend(); ++it) {
      if (!first) {
        std::cout << ' ';
      }
      std::cout << it->name;
      first = false;
    }
    std::cout << '\n';
  }
}

int main()
{
  gordejchik::List< gordejchik::NamedList > namedLists;
  gordejchik::readInput(namedLists);
  if (namedLists.empty()) {
    std::cout << 0 << '\n';
    return 0;
  }
  gordejchik::printNames(namedLists);
  return 0;
}
