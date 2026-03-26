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
  {
    data_.pushBack(value);
  }
  
  void push(T&& value)
  {
    data_.pushBack(std::move(value));
  }
  
  T pop()
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    T value = data_.front();
    data_.popFront();
    return value;
  }
  
  T& front()
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    return data_.front();
  }
  
  const T& front() const
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    return data_.front();
  }
  
  bool empty() const
  {
    return data_.empty();
  }
  
  size_t size() const
  {
    return data_.size();
  }
  
private:
  List< T > data_;
};
}

#endif
