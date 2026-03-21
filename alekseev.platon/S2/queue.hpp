#ifndef ALEKSEEV_S2_QUEUE_HPP
#define ALEKSEEV_S2_QUEUE_HPP

#include <cstddef>

namespace alekseev
{
  template< class T >
  class Queue
  {
  public:
    Queue() = default;
    Queue(const Queue< T >&) = default;
    Queue(Queue< T >&&) noexcept = default;
    Queue< T >& operator=(const Queue< T >&) = default;
    Queue< T >& operator=(Queue< T >&&) noexcept = default;
    ~Queue() = default;

    void push(const T& value);
    void push(T&& value);
    T drop();
    T& front();
    const T& front() const;
    bool empty() const;
    std::size_t size() const;

  private:
    std::size_t size_ = 0;
  };
}

#endif
