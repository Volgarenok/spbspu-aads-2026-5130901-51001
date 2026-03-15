#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include "node.hpp"

namespace hachaturyanov
{
  template< class T > class List {
    node head;
    size_t size;
  };
}

#endif
