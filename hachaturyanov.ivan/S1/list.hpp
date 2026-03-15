#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include "iterator.hpp"

namespace hachaturyanov
{
  template< class T > class List {
    node< T >* head_;
  public:
    size_t size;

    using iter = LIter< T >;
    using citer = LCIter< T >;

    List() noexcept;
    ~List();

    iter begin();
    iter end();
    citer begin() const;
    citer end() const;

    void addFront(const T& val);

    iter insertBefore(iter pos, const T& val);
    iter insertAfter(iter pos, const T& val);

    void popFront();
    void popEnd();

    iter erase(iter pos);

    void clear() noexcept;
    void clear(iter from, iter to) noexcept;

    List& operator=(List& other);
    List& operator=(List&& other);
  };


  template< class T > List< T >::List() noexcept:
  head_(nullptr),
  size(0)
  {}

  template< class T > typename List< T >::iter List< T >::begin() {
    if (size) {
      return iter(head_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::iter List< T >::end() {
    if (size) {
      return iter(head_->prev_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::citer List< T >::begin() const {
    if (size) {
      return citer(head_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::citer List< T >::end() const {
    if (size) {
      return citer(head_->prev_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > void List< T >::addFront(const T& val) {
    node< T >* tail_ = head_->prev_;
    node< T > newNode = new node< T >(val);
    newNode.next_ = head_;
    newNode.prev_ = tail_;
    tail_->next_ = newNode;
    head_->prev_ = newNode;
    head_ = newNode;
  }

}

#endif
