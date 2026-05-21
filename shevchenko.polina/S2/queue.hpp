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
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  void push(T&& value)
  {
    data_.pushBack(std::move(value));
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
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
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  T& front()
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    return data_.front();
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  const T& front() const
  {
    if (empty())
    {
      throw std::logic_error("queue is empty");
    }
    return data_.front();
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  bool empty() const
  {
    return data_.empty();
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
  size_t size() const
  {
    return data_.size();
  }
<<<<<<< HEAD
<<<<<<< HEAD

=======

>>>>>>> 4ca3688 (test)
=======

>>>>>>> dc1dfea (content)
private:
  List< T > data_;
};
}

#endif
