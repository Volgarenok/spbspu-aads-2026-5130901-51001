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
    List(const List& other);
    ~List();

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    size_t size() const noexcept;

    void pushFront(const T& value);
    void pushFront(T&& value);
    void pushBack(const T& value);
    void pushBack(T&& value);
    void popFront();
    void popBack();
    iterator erase(iterator pos);
    void clear() noexcept;

  private:
    using BaseNode = detail::BaseNode;
    using Node = detail::Node< T >;

    void insertBefore(BaseNode* pos, Node* node) noexcept;

    BaseNode fake_;
    size_t size_;
  };

  template< class T >
  List< T >::List() noexcept:
    fake_(),
    size_(0)
  {}

  template< class T >
  List< T >::List(const List& other):
    fake_(),
    size_(0)
  {
    try {
      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        pushBack(*it);
      }
    } catch (...) {
      clear();
      throw;
    }
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  void List< T >::insertBefore(BaseNode* pos, Node* node) noexcept
  {
    node->next_ = pos;
    node->prev_ = pos->prev_;
    pos->prev_->next_ = node;
    pos->prev_ = node;
    ++size_;
  }

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

  template< class T >
  void List< T >::pushFront(const T& value)
  {
    insertBefore(fake_.next_, new Node(value));
  }

  template< class T >
  void List< T >::pushFront(T&& value)
  {
    insertBefore(fake_.next_, new Node(static_cast< T&& >(value)));
  }

  template< class T >
  void List< T >::pushBack(const T& value)
  {
    insertBefore(&fake_, new Node(value));
  }

  template< class T >
  void List< T >::pushBack(T&& value)
  {
    insertBefore(&fake_, new Node(static_cast< T&& >(value)));
  }

  template< class T >
  void List< T >::popFront()
  {
    erase(begin());
  }

  template< class T >
  void List< T >::popBack()
  {
    erase(iterator(fake_.prev_));
  }

  template< class T >
  typename List< T >::iterator List< T >::erase(iterator pos)
  {
    BaseNode* node = pos.node_;
    BaseNode* next = node->next_;
    node->prev_->next_ = next;
    next->prev_ = node->prev_;
    delete static_cast< Node* >(node);
    --size_;
    return iterator(next);
  }

  template< class T >
  void List< T >::clear() noexcept
  {
    BaseNode* cur = fake_.next_;
    while (cur != &fake_) {
      BaseNode* next = cur->next_;
      delete static_cast< Node* >(cur);
      cur = next;
    }
    fake_.next_ = &fake_;
    fake_.prev_ = &fake_;
    size_ = 0;
  }
}

#endif
