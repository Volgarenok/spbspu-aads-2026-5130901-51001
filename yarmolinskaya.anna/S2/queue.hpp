#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "common/list.hpp"
#include <stdexcept>

namespace yarmolinskaya
{
  template< typename T >
  class Queue
  {
  public:
    bool empty() const noexcept
    {
      return data_.empty();
    }

    void push(const T& value)
    {
      data_.push_back(value);
    }

    void pop()
    {
      if (empty())
      {
        throw std::logic_error("queue empty");
      }

      data_.pop_front();
    }

    T& front()
    {
      if (empty())
      {
        throw std::logic_error("queue empty");
      }

      return data_.front();
    }

    const T& front() const
    {
      if (empty())
      {
        throw std::logic_error("queue empty");
      }

      return data_.front();
    }

  private:
    List< T > data_;
  };
}

#endif
