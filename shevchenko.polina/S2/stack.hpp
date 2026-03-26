#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace shevchenko
{
template< class T >
class Stack
{
public:
  void push(const T& value)
  {
    data_.pushFront(value);
  }
  
  void push(T&& value)
  {
    data_.pushFront(std::move(value));
  }
  
  T pop()
  {
    if (empty())
    {
      throw std::logic_error("stack is empty");
    }
    T value = data_.front();
    data_.popFront();
    return value;
  }
  
  T& top()
  {
    if (empty())
    {
      throw std::logic_error("stack is empty");
    }
    return data_.front();
  }
  
  const T& top() const
  {
    if (empty())
    {
      throw std::logic_error("stack is empty");
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
