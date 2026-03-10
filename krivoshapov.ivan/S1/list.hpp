#ifndef KRIVOSHAPOV_LIST_HPP
#define KRIVOSHAPOV_LIST_HPP

#include "iterator.hpp"
#include "node.hpp"

namespace krivoshapov
{
  template <class T>
  class List
  {
  public:
    using iterator = LIter<T>;
    using const_iterator = LCIter<T>;

    List() : head_(nullptr),
             tail_(nullptr),
             size_(0)
    {
    }

    List(const List &other) : head_(nullptr),
                              tail_(nullptr),
                              size_(0)
    {
      try
      {
        for (auto it = other.cbegin(); it != other.cend(); ++it)
        {
          pushBack(*it);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }
  };
}
#endif
