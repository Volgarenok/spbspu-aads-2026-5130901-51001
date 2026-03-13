#ifndef NODE_HPP
#define NODE_HPP

namespace vishnaykov
{
  template< class T >
  struct Node
  {
    T data_;
    *next_;

    Node(T val):
      data_(val),
      next_(nullptr)
    {}
  };
}

#endif