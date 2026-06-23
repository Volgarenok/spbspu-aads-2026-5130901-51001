#ifndef STACK_HPP
#define STACK_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace karpenko
{
  template < typename T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack &) = default;
    Stack(Stack &&) = default;
    Stack & operator=(const Stack &) = default;
    Stack & operator=(Stack &&) = default;

    bool empty() const noexcept
    {
      return list_.empty();
    }

    size_t size() const noexcept
    {
      return list_.size();
    }

    T & top()
    {
      if (empty())
        throw std::out_of_range("Stack::top(): empty stack");
      return list_.front();
    }

    const T & top() const
    {
      if (empty())
        throw std::out_of_range("Stack::top(): empty stack");
      return list_.front();
    }

    void push(const T & value)
    {
      list_.pushFront(value);
    }

    void push(T && value)
    {
      list_.pushFront(std::move(value));
    }

    template< typename... Args >
    void emplace(Args &&... args)
    {
      list_.emplaceFront(std::forward< Args >(args)...);
    }

    void pop()
    {
      if (empty())
        throw std::out_of_range("Stack::pop(): empty stack");
      list_.popFront();
    }

    void clear() noexcept
    {
      list_.clear();
    }

    void swap(Stack & other) noexcept
    {
      list_.swap(other.list_);
    }

  private:
    List< T > list_;
  };

  template < typename T >
  void swap(Stack< T > & lhs, Stack< T > & rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif
