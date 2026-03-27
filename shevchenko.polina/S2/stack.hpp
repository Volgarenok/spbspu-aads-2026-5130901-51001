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
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  void push(T&& value)
  {
    data_.pushFront(std::move(value));
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
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
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  T& top()
  {
    if (empty())
    {
      throw std::logic_error("stack is empty");
    }
    return data_.front();
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  const T& top() const
  {
    if (empty())
    {
      throw std::logic_error("stack is empty");
    }
    return data_.front();
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  bool empty() const
  {
    return data_.empty();
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  size_t size() const
  {
    return data_.size();
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
private:
  List< T > data_;
};
}

#endif
