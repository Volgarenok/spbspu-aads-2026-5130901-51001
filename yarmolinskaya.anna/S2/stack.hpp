#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>

#include "common/list.hpp"

namespace yarmolinskaya
{
  template< typename T >
  class Stack
  {
  public:
    bool empty() const noexcept
    {
      return data_.empty();
    }

    void push(const T& value)
    {
      data_.push_front(value);
    }

    void pop()
    {
      if (empty())
      {
        throw std::logic_error("stack empty");
      }

      data_.pop_front();
    }

    T& top()
    {
      if (empty())
      {
        throw std::logic_error("stack empty");
      }

      return data_.front();
    }

    const T& top() const
    {
      if (empty())
      {
        throw std::logic_error("stack empty");
      }

      return data_.front();
    }

  private:
    List< T > data_;
  };
}

#endif
