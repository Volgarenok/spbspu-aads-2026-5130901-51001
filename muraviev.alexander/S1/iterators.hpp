#ifndef ITERATORS_HPP
#define ITERATORS_HPP

#include "node.hpp"

namespace muraviev
{
  template< class T >
  class List;

  template< class T >
  class LCIter;

  template< class T >
  class LIter
  {
    friend class List< T >;
    friend class LCIter< T >;
  public:
    LIter();

    T& operator*() const;
    T* operator->() const;

    LIter& operator++();

    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;

  private:
    explicit LIter(Node< T >* node, Node< T >* head);

    Node< T >* node_;
    Node< T >* head_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter();
    LCIter(const LIter< T >& other);

    const T& operator*() const;
    const T* operator->() const;

    LCIter& operator++();

    bool operator==(const LCIter& other) const;
    bool operator!=(const LCIter& other) const;

  private:
    explicit LCIter(const Node< T >* node, const Node< T >* head);

    const Node< T >* node_;
    const Node< T >* head_;
  };
}

#endif
