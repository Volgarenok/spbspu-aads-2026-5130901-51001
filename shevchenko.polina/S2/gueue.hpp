#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace shevchenko
{
template< class T >
class Queue
{
public:
  void push(const T& value)
  void push(T&& value)
  T pop()
  T& front()
  const T& front() const
  bool empty() const
  size_t size() const
  
private:
  List< T > data_;
};
}

#endif
