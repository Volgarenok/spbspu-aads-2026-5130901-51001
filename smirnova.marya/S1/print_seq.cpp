#include "print_seq.hpp"
#include <iostream>
#include <limits>

namespace smirnova
{
  template < class T >
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

  template < class T >
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

  bool processSequences(const List<Seq>& sequences)
  {
    if(sequences.empty()) {
      std::cout << "0\n";
      return true;
    }

    bool firstName = true;
    for(LCIter< Seq > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(!firstName) {
        std::cout << " ";
      }
      std::cout << it.value().name;
      firstName = false;
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for(LCIter< Seq > it = sequences.cbegin(); it.valid(); it.next())
    {
      if(it.value().numbers.size() > maxLen) {
        maxLen = it.value().numbers.size();
      }
    }

    for(size_t row = 0; row < maxLen; ++row)
    {
      bool firstNum = true;
      for(LCIter< Seq > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().numbers.size()) {
          LCIter< size_t > numIt = getElementAt(it.value().numbers, row);
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
      for(LCIter< Seq > it = sequences.cbegin(); it.valid(); it.next())
      {
        if(row < it.value().numbers.size()) {
          size_t val = getElementAt(it.value().numbers, row).value();
          if(willOverflow(sum, val)) {
            std::cerr << "overflow\n";
            return false;
          }
          sum += val;
        }
      }
      sums.pushBack(sum);
    }

    printList(sums);
    return true;
  }
}

