#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "node.hpp"

namespace alekseev
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
    explicit LIter(detail::NodeBase * node);

    T & operator*() const;
    T * operator->() const;
    LIter & operator++();
    LIter operator++(int);
    bool operator==(const LIter & rhs) const;
    bool operator!=(const LIter & rhs) const;

  private:
    detail::NodeBase * node_;
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
  public:
    LCIter();
    explicit LCIter(const detail::NodeBase * node);
    LCIter(const LIter< T > & other);

    const T & operator*() const;
    const T * operator->() const;
    LCIter & operator++();
    LCIter operator++(int);
    bool operator==(const LCIter & rhs) const;
    bool operator!=(const LCIter & rhs) const;

  private:
    const detail::NodeBase * node_;
  };
}

#endif
