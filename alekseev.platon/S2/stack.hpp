#ifndef ALEKSEEV_S2_STACK_HPP
#define ALEKSEEV_S2_STACK_HPP

#include <cstddef>

namespace alekseev
{
  template< class T >
  class Stack
  {
  public:
    Stack() = default;
    Stack(const Stack< T >&) = default;
    Stack(Stack< T >&&) noexcept = default;
    Stack< T >& operator=(const Stack< T >&) = default;
    Stack< T >& operator=(Stack< T >&&) noexcept = default;
    ~Stack() = default;

    void push(const T& value);
    void push(T&& value);
    T drop();
    T& top();
    const T& top() const;
    bool empty() const;
    std::size_t size() const;

  private:
    std::size_t size_ = 0;
  };
}

#endif
