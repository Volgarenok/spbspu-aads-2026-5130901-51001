#ifndef LIST_HPP
#define LIST_HPP

#include "iterators.hpp"

namespace muraviev
{
  template< class T >
  class List
  {
  public:
    using iter = LIter< T >;
    using c_iter = LCIter< T >;

    List();
    List(const List& other);
    List& operator=(const List& other);
    ~List();

    iter begin();
    c_iter begin() const;

    iter end();
    c_iter end() const;

    iter last();
    c_iter last() const;

    bool empty() const;

    void pushFront(const T& value);
    void insert(iter pos, const T& value);
    void popFront();
    void popBack();
    iter erase(iter pos);
    void clear();

  private:
    Node< T >* head_;
    Node< T >* tail_;
  };
}

#endif
