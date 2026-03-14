#include <iostream>
#include "list.hpp"
#include <string>
#include <utility>

int main() {
  volkovich::List<std::pair< std::string, volkovich::List<size_t> > > data;

  std::string name;

  while (std::cin >> name) {
    volkovich::List<size_t> numbers;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF) {
      size_t number;
      std::cin >> number;
      numbers.pushBack(number);
    }
    data.pushBack(std::make_pair(name, numbers));
   }
}

