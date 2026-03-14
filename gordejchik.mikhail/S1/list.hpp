#ifndef GORDEJCHIK_LIST_HPP
#define GORDEJCHIK_LIST_HPP

#include <iostream>
#include "iterator.hpp"

namespace gordejchik {
  template< class T >
  class List {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List() noexcept;
    ~List();

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    using BaseNode = detail::BaseNode;
    using Node = detail::Node< T >;

    BaseNode fake_;
    size_t size_;
  };

  template< class T >
  List< T >::List() noexcept:
    fake_(),
    size_(0)
  {}

  template< class T >
  List< T >::~List()
  {}

  template< class T >
  typename List< T >::iterator List< T >::begin() noexcept
  {
    return iterator(fake_.next_);
  }

  template< class T >
  typename List< T >::iterator List< T >::end() noexcept
  {
    return iterator(&fake_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::begin() const noexcept
  {
    return const_iterator(fake_.next_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::end() const noexcept
  {
    return const_iterator(&fake_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cbegin() const noexcept
  {
    return const_iterator(fake_.next_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cend() const noexcept
  {
    return const_iterator(&fake_);
  }

  template< class T >
  bool List< T >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class T >
  size_t List< T >::size() const noexcept
  {
    return size_;
  }
}

#endif
