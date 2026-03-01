#ifndef LIST_HPP
#define LIST_HPP

#include "iterator.hpp"

namespace alekseev
{
  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List();
    ~List();

    List(const List & other);
    List(List && other);
    List & operator=(const List & other);
    List & operator=(List && other);

    void swap(List & other);

    iterator before_begin();
    const_iterator cbefore_begin() const;

    iterator begin();
    iterator end();
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool empty() const;
    T & front();
    const T & front() const;

    void push_front(const T & value);
    void push_front(T && value);
    void pop_front();
    void clear();

    iterator insert_after(iterator pos, const T & value);
    iterator insert_after(iterator pos, T && value);
    iterator erase_after(iterator pos);

  private:
    detail::NodeBase sentinel_;
  };

  template< class T >
  List< T >::List()
  {
    sentinel_.next = nullptr;
  }

  template< class T >
  List< T >::~List()
  {
    clear();
  }

  template< class T >
  List< T >::List(const List & other):
    List()
  {
    iterator tail = before_begin();
    for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
    {
      tail = insert_after(tail, *it);
    }
  }

  template< class T >
  List< T >::List(List && other):
    List()
  {
    swap(other);
  }

  template< class T >
  List< T > & List< T >::operator=(const List & other)
  {
    List tmp(other);
    swap(tmp);
    return *this;
  }

  template< class T >
  List< T > & List< T >::operator=(List && other)
  {
    List tmp(static_cast< List && >(other));
    swap(tmp);
    return *this;
  }

  template< class T >
  typename List< T >::iterator List< T >::before_begin()
  {
    return iterator(&sentinel_);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cbefore_begin() const
  {
    return const_iterator(&sentinel_);
  }

  template< class T >
  typename List< T >::iterator List< T >::begin()
  {
    return iterator(sentinel_.next);
  }

  template< class T >
  typename List< T >::iterator List< T >::end()
  {
    return iterator(nullptr);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cbegin() const
  {
    return const_iterator(sentinel_.next);
  }

  template< class T >
  typename List< T >::const_iterator List< T >::cend() const
  {
    return const_iterator(nullptr);
  }

  template< class T >
  bool List< T >::empty() const
  {
    return sentinel_.next == nullptr;
  }

  template< class T >
  T & List< T >::front()
  {
    return static_cast< detail::Node< T > * >(sentinel_.next)->data;
  }

  template< class T >
  const T & List< T >::front() const
  {
    return static_cast< const detail::Node< T > * >(sentinel_.next)->data;
  }

  template< class T >
  void List< T >::push_front(const T & value)
  {
    detail::Node< T > * node = new detail::Node< T >(value);
    node->next = sentinel_.next;
    sentinel_.next = node;
  }

  template< class T >
  void List< T >::push_front(T && value)
  {
    detail::Node< T > * node = new detail::Node< T >(static_cast< T && >(value));
    node->next = sentinel_.next;
    sentinel_.next = node;
  }

  template< class T >
  void List< T >::pop_front()
  {
    detail::NodeBase * old = sentinel_.next;
    sentinel_.next = old->next;
    delete static_cast< detail::Node< T > * >(old);
  }

  template< class T >
  void List< T >::clear()
  {
    while (sentinel_.next != nullptr)
    {
      pop_front();
    }
  }

  template< class T >
  typename List< T >::iterator List< T >::insert_after(iterator pos, const T & value)
  {
    detail::Node< T > * node = new detail::Node< T >(value);
    node->next = pos.node_->next;
    pos.node_->next = node;
    return iterator(node);
  }

  template< class T >
  typename List< T >::iterator List< T >::insert_after(iterator pos, T && value)
  {
    detail::Node< T > * node = new detail::Node< T >(static_cast< T && >(value));
    node->next = pos.node_->next;
    pos.node_->next = node;
    return iterator(node);
  }

  template< class T >
  typename List< T >::iterator List< T >::erase_after(iterator pos)
  {
    detail::NodeBase * target = pos.node_->next;
    pos.node_->next = target->next;
    delete static_cast< detail::Node< T > * >(target);
    return iterator(pos.node_->next);
  }

  template< class T >
  void List< T >::swap(List & other)
  {
    detail::NodeBase * tmp = sentinel_.next;
    sentinel_.next = other.sentinel_.next;
    other.sentinel_.next = tmp;
  }
}

#endif
