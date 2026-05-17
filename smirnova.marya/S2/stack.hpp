#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace smirnova
{
  template < typename T >
  class Stack {
  private:
    List< T > data;

  public:
    void push(const T& value)
    {
      data.pushBack(value);
    }

    T drop() {
      if (data.empty())
        throw std::logic_error("stack is empty");

      T val = data.back();
      data.popBack();
      return val;
    }

    T& top()
    {
      if (data.empty())
        throw std::logic_error("stack is empty");

      return data.back();
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

    T popBack()
    {
      return drop();
    }
  };

}

#endif

