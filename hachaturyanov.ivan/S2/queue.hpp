#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"

namespace hachaturyanov
{

  template< class T > class Queue {
    List< T > data_;
  public:
    void push(const T &value);
    T drop();
  };


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
