#ifndef STACK_HPP
#define STACK_HPP

#include "../common/list.hpp"

namespace hachaturyanov
{

  template< class T > class Stack {
    List< T > data_;
  public:
    void push(const T &value);
    void push(T &&value);
    T drop();

    T& begin();
    const T& begin() const;
    T& end();
    const T& end() const;

    size_t size() const noexcept;
    bool isEmpty() const noexcept;
  };

  template< class T > bool Stack< T >::isEmpty() const noexcept
  {
    return data_.isEmpty();
  }

  template< class T > size_t Stack< T >::size() const noexcept
  {
    return data_.size();
  }

  template< class T > const T& Stack< T >::end() const
  {
    auto it = data_.end();
    --it;
    return *it;
  }

  template< class T > T& Stack< T >::end()
  {
    auto it = data_.end();
    --it;
    return *it;
  }

  template< class T > const T& Stack< T >::begin() const
  {
    return *(data_.begin());
  }

  template< class T > T& Stack< T >::begin()
  {
    return *(data_.begin());
  }

  template< class T > void Stack< T >::push(const T &value)
  {
    data_.addFront(value);
  }

  template< class T > void Stack< T >::push(T &&value)
  {
    data_.addFront(std::move(value));
  }

  template< class T > T Stack< T >::drop()
  {
    if (data_.isEmpty()) {
      throw std::logic_error("Stack is empty");
    }
    T res = std::move(begin());
    data_.popFront();
    return res;
  }


}


#endif
