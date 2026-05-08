#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace smirnova
{
  template < typename T >
  class Queue {
  private:
    List< T > data;

  public:
    void push(const T& value)
    {
      data.pushBack(value);
    }

    T drop()
    {
      if (data.empty())
        throw std::logic_error("queue is empty");

      T val = data.front();
      data.popFront();
      return val;
    }

    bool empty() const
    {
      return data.empty();
    }

    size_t size() const
    {
      return data.size();
    }

    void pushBack(const T& value)
    {
      push(value);
    }

    T popFront()
    {
      return drop();
    }

    T& front()
    {
      if (data.empty())
        throw std::logic_error("queue is empty");

      return data.front();
    }

    const T& front() const
    {
      if (data.empty())
        throw std::logic_error("queue is empty");

      return data.front();
    }
  };

}

#endif

