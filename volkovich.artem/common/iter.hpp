#ifndef ITER_HPP
#define ITER_HPP
#include "item.hpp"

namespace volkovich
{
  template <class T>
  class LIter
  {

  public:
    Item<T> *item;
    explicit LIter(Item<T> *v) : item(v) {}
    LIter &operator++() noexcept
    {
      item = item->next;
      return *this;
    };
    LIter &operator++(int) noexcept
    {
      LIter tmp = *this;
      item = item->next;
      return tmp;
    };

    bool operator==(const LIter &other) const noexcept
    {
      return item == other.item;
    };

    bool operator!=(const LIter &other) const noexcept
    {
      return item != other.item;
    };
    T &operator*()
    {
      return item->data;
    }
    T *operator->()
    {
      return &item->data;
    }
  };

  template <class T>
  class LCIter
  {
  private:
    const Item<T> *item;

  public:
    explicit LCIter(Item<T> *v) : item(v) {}
    LCIter &operator++() noexcept
    {
      item = item->next;
      return *this;
    };
    LCIter &operator++(int) noexcept
    {
      LCIter tmp = *this;
      item = item->next;
      return tmp;
    };

    bool operator==(const LCIter &other) const noexcept
    {
      return item == other.item;
    };

    bool operator!=(const LCIter &other) const noexcept
    {
      return item != other.item;
    };

    const T &operator*()
    {
      return item->data;
    }

    const T *operator->() const
    {
      return &item->data;
    }
  };
}

#endif
