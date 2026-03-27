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
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  void push(T&& value)
  {
    data_.pushBack(std::move(value));
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
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
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  T& front()
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    return data_.front();
  }
<<<<<<< HEAD

=======
  
>>>>>>> 4ca3688 (test)
  const T& front() const
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
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
