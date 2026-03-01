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
}

#endif
