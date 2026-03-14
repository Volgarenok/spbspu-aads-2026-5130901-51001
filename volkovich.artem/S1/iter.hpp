#ifndef ITER_HPP
#define ITER_HPP
#include "item.hpp"

namespace volkovich
{
  template <class T>
  class LCIter
  {
  };

  template <class T>
  class LIter
  {
  private:
    Item<T> *item;

  public:
    explicit LIter(Item<T> *v);
    LIter &operator++() noexcept;
    LIter &operator++(int) noexcept;
    bool operator==(const LIter &other) const noexcept;
    bool operator!=(const LIter &other) const noexcept;
  };
}

#endif
