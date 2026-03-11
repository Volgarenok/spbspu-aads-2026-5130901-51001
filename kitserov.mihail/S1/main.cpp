#include <limits>
#include <iostream>
#include <string>
#include "list.hpp"
#include "utils.hpp"

int main()
{
  using namespace kitserov;
  List< std::string > names;
  List< List< int > > listOfLists;
  std::string name;
  while (true) {
    try {
      if (!(std::cin >> name)) {
        break;
      }
      names.insert_tail(name);
      List< int > numbers;
      int num;
      while (std::cin >> num) {
        numbers.insert_tail(num);
      }
      if (std::cin.eof()) {
        listOfLists.insert_tail(numbers);
        numbers.clear();
        break;
      }
      if (std::cin.fail()) {
        std::cin.clear();
        std::cin >> std::ws;
        int nextChar = std::cin.peek();
        if (nextChar != EOF && std::isdigit(static_cast< unsigned char >(nextChar))) {
          std::cerr << "Overflow\n";
          numbers.clear();
          names.clear();
          clearListOfLists(listOfLists);
          return 1;
        }
        listOfLists.insert_tail(numbers);
      }
      numbers.clear();
    } catch (...) {
      names.clear();
      clearListOfLists(listOfLists);
      std::cerr << "bad allocated memory\n";
      return 2;
    }
  }
  if (names.get_size() == 0) {
    std::cout << "0\n";
    return 0;
  }
  std::cout << "\n";
  printList(names);
  size_t maxSize = 0;
  for (LIter< List< int > > it = listOfLists.begin(); it != listOfLists.end(); ++it) {
    size_t s = it->get_size();
    if (s > maxSize) {
      maxSize = s;
    }
  }
  List< int > summes;
  for (size_t i = 0; i < maxSize; i++) {
    int sum = 0;
    for (size_t j = 0; j < listOfLists.get_size(); j++) {
      List< int >& curList = *(listOfLists[j]);
      if (curList.get_size() > i) {
        int val = *(curList[i]);
        std::cout << val << " ";
        if (sum > std::numeric_limits< int >::max() - val) {
          std::cerr << "Overflow sum\n";
          summes.clear();
          names.clear();
          clearListOfLists(listOfLists);
          return 1;
        }
        sum += val;
      }
    }
    summes.insert_tail(sum);
    std::cout << "\n";
  }
  printList(summes);
  summes.clear();
  names.clear();
  clearListOfLists(listOfLists);
  return 0;
}
