#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace hachaturyanov
{
  struct nodebase {
    nodebase* next_;
    nodebase* prev_;

    nodebase():
     next_(this),
     prev_(this)
    {}
  };

  template< class T > struct node: nodebase {
    T val_;

    explicit node(const T &val):
     nodebase(),
     val_(val)
    {}

    explicit node(T &&val):
     nodebase(),
     val_(std::move(val))
    {}
  };
}

#endif
