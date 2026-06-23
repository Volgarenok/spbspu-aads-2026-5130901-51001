#ifndef QUEUE_HPP
#define QUEUE_HPP

#include <stdexcept>
#include "../common/list.hpp"

namespace karpenko
{
  template < typename T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue &) = default;
    Queue(Queue &&) = default;
    Queue & operator=(const Queue &) = default;
    Queue & operator=(Queue &&) = default;

    bool empty() const noexcept
    {
      return list_.empty();
    }

    size_t size() const noexcept
    {
      return list_.size();
    }

    T & front()
    {
      if (empty())
        throw std::out_of_range("Queue::front(): empty queue");
      return list_.front();
    }

    const T & front() const
    {
      if (empty())
        throw std::out_of_range("Queue::front(): empty queue");
      return list_.front();
    }

    T & back()
    {
      if (empty())
        throw std::out_of_range("Queue::back(): empty queue");
      return list_.back();
    }

    const T & back() const
    {
      if (empty())
        throw std::out_of_range("Queue::back(): empty queue");
      return list_.back();
    }

    void push(const T & value)
    {
      list_.pushBack(value);
    }

    void push(T && value)
    {
      list_.pushBack(std::move(value));
    }

    template< typename... Args >
    void emplace(Args &&... args)
    {
      list_.emplaceBack(std::forward< Args >(args)...);
    }

    void pop()
    {
      if (empty())
        throw std::out_of_range("Queue::pop(): empty queue");
      list_.popFront();
    }

    void clear() noexcept
    {
      list_.clear();
    }

    void swap(Queue & other) noexcept
    {
      list_.swap(other.list_);
    }

  private:
    List< T > list_;
  };

  template < typename T >
  void swap(Queue< T > & lhs, Queue< T > & rhs) noexcept
  {
    lhs.swap(rhs);
  }
}

#endif
