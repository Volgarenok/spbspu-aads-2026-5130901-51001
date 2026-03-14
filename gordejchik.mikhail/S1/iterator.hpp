#ifndef GORDEJCHIK_ITERATOR_HPP
#define GORDEJCHIK_ITERATOR_HPP

#include "node.hpp"

namespace gordejchik {
  template< class T >
  class List;

  template< class T >
  class LCIter;

  template< class T >
  class LIter {
    friend class List< T >;
    friend class LCIter< T >;
  public:
    LIter();
    T& operator*() const;
    T* operator->() const;
    LIter& operator++();
    LIter operator++(int);
    LIter& operator--();
    LIter operator--(int);
    bool operator==(const LIter& rhs) const;
    bool operator!=(const LIter& rhs) const;
  private:
    using BaseNode = detail::BaseNode;
    using Node = detail::Node< T >;
    explicit LIter(BaseNode* node);
    BaseNode* node_;
  };

  template< class T >
  class LCIter {
    friend class List< T >;
  public:
    LCIter();
    LCIter(const LIter< T >& other);
    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++();
    LCIter operator++(int);
    LCIter& operator--();
    LCIter operator--(int);
    bool operator==(const LCIter& rhs) const;
    bool operator!=(const LCIter& rhs) const;
  private:
    using BaseNode = detail::BaseNode;
    using Node = detail::Node< T >;
    explicit LCIter(const BaseNode* node);
    const BaseNode* node_;
  };
}

#endif
