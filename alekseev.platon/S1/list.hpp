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
}

#endif
