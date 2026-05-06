#ifndef NODE_HPP
#define NODE_HPP

namespace alekseev
{
  namespace detail
  {
    struct NodeBase
    {
      NodeBase * next;
      NodeBase(): next(nullptr) {}
    };

    template< class T >
    struct Node: NodeBase
    {
      T data;
      explicit Node(const T & val): NodeBase(), data(val) {}
      explicit Node(T && val): NodeBase(), data(static_cast< T && >(val)) {}
    };
  }
}

#endif
