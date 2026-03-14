#ifndef ITEM_HPP
#define ITEM_HPP

namespace volkovich {
  template <class T>
    class Item
    {
    public:
      T data;
      Item<T> *next = nullptr;
      explicit Item(const T &v) : data(v) {};
      explicit Item(T &&v) : data(std::move(v)) {};
      explicit Item() {};
    };
}

#endif
