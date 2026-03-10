#ifndef KRIVOSHAPOV_NODE_HPP
#define KRIVOSHAPOV_NODE_HPP

namespace krivoshapov
{
  namespace detail
  {

    template <class T>
    struct Node
    {
      T value_;
      Node *next_;
      Node *prev_;
    }
  }
}

#endif
