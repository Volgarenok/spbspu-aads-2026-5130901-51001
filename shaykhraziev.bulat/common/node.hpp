#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

namespace shaykhraziev
{
  struct NodeBase
  {
    NodeBase* next_;

    NodeBase() :
      next_(nullptr)
    {}
  };

  template< class T >
  struct Node : public NodeBase
  {
    T data_;

    explicit Node(const T& val) :
      NodeBase(),
      data_(val)
    {}

    explicit Node(T&& val) :
      NodeBase(),
      data_(std::move(val))
    {}
  };
}

#endif

