#ifndef BORISOV_STACK_HPP
#define BORISOV_STACK_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace borisov
{
  template < typename T >
  class Stack
  {
  public:
    void push(const T& value);
    T& top();
    const T& top() const;
    void pop();
    bool empty() const;
    std::size_t size() const;
    void clear();

  private:
    List< T > list_;
  };

  template < typename T >
  void Stack< T >::push(const T& value)
  {
    list_.pushFront(value);
  }

  template < typename T >
  T& Stack< T >::top()
  {
    if (list_.empty())
    {
      throw std::logic_error("Stack::top on empty stack");
    }
    return list_.front();
  }

  template < typename T >
  const T& Stack< T >::top() const
  {
    if (list_.empty())
    {
      throw std::logic_error("Stack::top on empty stack");
    }
    return list_.front();
  }

  template < typename T >
  void Stack< T >::pop()
  {
    if (list_.empty())
    {
      throw std::logic_error("Stack::pop on empty stack");
    }
    list_.popFront();
  }

  template < typename T >
  bool Stack< T >::empty() const
  {
    return list_.empty();
  }

  template < typename T >
  std::size_t Stack< T >::size() const
  {
    return list_.size();
  }

  template < typename T >
  void Stack< T >::clear()
  {
    list_.clear();
  }
}

#endif
