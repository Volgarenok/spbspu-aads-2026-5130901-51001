#ifndef BORISOV_QUEUE_HPP
#define BORISOV_QUEUE_HPP

#include "../common/list.hpp"
#include <stdexcept>

namespace borisov
{
  template < typename T >
  class Queue
  {
  public:
    void push(const T& value);
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;
    void pop();
    bool empty() const;

  private:
    List< T > list_;
  };

  template < typename T >
  void Queue< T >::push(const T& value)
  {
    list_.pushBack(value);
  }

  template < typename T >
  T& Queue< T >::front()
  {
    if (list_.empty())
    {
      throw std::logic_error("Queue::front on empty queue");
    }
    return list_.front();
  }

  template < typename T >
  const T& Queue< T >::front() const
  {
    if (list_.empty())
    {
      throw std::logic_error("Queue::front on empty queue");
    }
    return list_.front();
  }

  template < typename T >
  T& Queue< T >::back()
  {
    if (list_.empty())
    {
      throw std::logic_error("Queue::back on empty queue");
    }
    return list_.back();
  }

  template < typename T >
  const T& Queue< T >::back() const
  {
    if (list_.empty())
    {
      throw std::logic_error("Queue::back on empty queue");
    }
    return list_.back();
  }

  template < typename T >
  void Queue< T >::pop()
  {
    if (list_.empty())
    {
      throw std::logic_error("Queue::pop on empty queue");
    }
    list_.popFront();
  }

  template < typename T >
  bool Queue< T >::empty() const
  {
    return list_.empty();
  }
}

#endif
