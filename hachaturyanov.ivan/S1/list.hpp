#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include "node.hpp"

namespace hachaturyanov
{
  template< class T > class List {
  private:
    nodebase fakehead;
    size_t size;
  };
}

#endif
