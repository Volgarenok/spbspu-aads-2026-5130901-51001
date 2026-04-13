#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"

namespace hachaturyanov
{

  template< class T > class Queue {
    List< T > data_;
  public:
    void push(const T &value);
    void push(T &&value);
    T drop();

    size_t size() const noexcept;
    bool isEmpty() const noexcept;
  };

template< class T > void Queue< T >::push(T &&value)
{
  data_.addEnd(std::move(value));
}

template< class T > bool Queue< T >::isEmpty() const noexcept
{
  return data_.isEmpty();
}

template< class T > size_t Queue< T >::size() const noexcept
{
  return data_.size();
}

template< class T > void Queue< T >::push(const T &value)
{
  data_.addEnd(value);
}

template< class T > T Queue< T >::drop()
{
  if (data_.isEmpty()) {
    throw std::logic_error("Queue is empty");
  }
  T res = *(data_.begin());
  data_.popFront();
  return res;
}

}

#endif
