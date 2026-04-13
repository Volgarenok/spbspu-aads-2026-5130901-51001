#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace hachaturyanov
{
  template< class T > struct node {
    T val_;
    node* next_;
    node* prev_;

    explicit node(const T &val):
     val_(val),
     next_(nullptr),
     prev_(nullptr)
    {}

    explicit node(T &&val):
     val_(std::move(val)),
     next_(nullptr),
     prev_(nullptr)
    {}
  };
}

#endif
