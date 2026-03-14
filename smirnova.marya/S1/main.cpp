#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>
#include "list.hpp"

namespace smirnova
{
  template < typename T >
  LCIter< T > getElementAt(const List< T >& lst, size_t idx)
  {
    LCIter< T > it = lst.cbegin();
    size_t i = 0;
    while(it.valid() && i < idx) {
      it.next();
      ++i;
    }
    return it;
  }

  template < typename T >
  void printList(const List< T >& lst)
  {
    if(lst.empty()) {
      std::cout << "0\n";
      return;
    }
    bool first = true;
    for(LCIter< T > it = lst.cbegin(); it.valid(); it.next())
    {
      if(!first) {
        std::cout << " ";
      }
      std::cout << it.value();
      first = false;
    }
    std::cout << "\n";
  }

  bool willOverflow(size_t a, size_t b)
  {
    return b > std::numeric_limits< size_t >::max() - a;
  }

  bool processSequences(const List< std::pair< std::string, List< size_t > > >& sequences)
  {
    if(sequences.empty()) {
      std::cout << "0\n";
      return true;
    }

    bool firstName = true;
    for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(!firstName) {
        std::cout << " ";
      }
      std::cout << it.value().first;
      firstName = false;
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(it.value().second.size() > maxLen) {
        maxLen = it.value().second.size();
      }
    }

    for(size_t row = 0; row < maxLen; ++row)
    {
      bool firstNum = true;
      for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().second.size()) {
          LCIter< size_t > numIt = getElementAt(it.value().second, row);
          if(!firstNum) {
            std::cout << " ";
          }
          std::cout << numIt.value();
          firstNum = false;
        }
      }
      if(!firstNum) {
        std::cout << "\n";
      }
    }

    List< size_t > sums;
    for(size_t row = 0; row < maxLen; ++row)
    {
      size_t sum = 0;
      for(LCIter< std::pair< std::string, List< size_t > > > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().second.size()) {
          size_t val = getElementAt(it.value().second, row).value();
          if(willOverflow(sum, val)) {
            std::cerr << "overflow\n";
            return false;
          }
          sum += val;
        }
      }
      sums.push_back(sum);
    }

    printList(sums);
    return true;
  }
}

int main() {
  using namespace smirnova;

  List< std::pair< std::string, List< size_t > > > sequences;
  bool overflowDetected = false;

  std::string name;

  while (std::cin >> name) {
      List< size_t > numbers;

      while (true) {
          int c = std::cin.peek();
          if (c == '\n' || c == '\r' || c == EOF) {
              break;
          }

          size_t num = 0;

          if (!(std::cin >> num)) {
              overflowDetected = true;
              std::cin.clear();
              std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
              break;
          }

          numbers.push_back(num);
      }

      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
      sequences.push_back({name, std::move(numbers)});
  }

  if (overflowDetected) {
    std::cerr << "overflow\n";
    return 1;
  }

  if (!processSequences(sequences)) {
    return 1;
  }

  return 0;
}

