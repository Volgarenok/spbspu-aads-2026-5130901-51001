#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "list.hpp"

namespace kitserov
{
  template< class T >
  void printList(List< T >& list)
  {
    for (LIter< T > it = list.begin(); it != list.end(); ++it) {
      std::cout << *it << " ";
    }
    std::cout << "\n";
  }

  template< class T >
  void clearListOfLists(List< List< T > >& listOfLists)
  {
    for (LIter< List< T > > it = listOfLists.begin(); it != listOfLists.end(); ++it) {
      it->clear();
    }
    listOfLists.clear();
  }
}

#endif
