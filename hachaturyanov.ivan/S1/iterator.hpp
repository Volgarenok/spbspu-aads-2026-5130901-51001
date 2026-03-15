#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "node.hpp"

namespace hachaturyanov
{
  template< class T > class List;
  template< class T > class LIter {
    friend class List< T >;
    explicit LIter(node< T >* n):
     node_(n)
    {}
    node< T >* node_;
  public:
    T& operator*();
    T* operator->();
    LIter& operator++();
    LIter& operator--();
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
  };

  template< class T > T& LIter< T >::operator*() {
    return *node_;
  }

  template< class T > T* LIter< T >::operator->() {
    return &(node_->val_);
  }

  template< class T > LIter< T >& LIter< T >::operator++() {
    node_ = node_->next_;
    return *this;
  }

  template< class T > LIter< T >& LIter< T >::operator--() {
    node_ = node_->prev_;
    return *this;
  }

  template< class T > bool LIter< T >::operator==(const LIter< T >& other) const {
    return node_ == other.node_;
  }

  template< class T > bool LIter< T >::operator!=(const LIter< T >& other) const {
    return node_ != other.node_;
  }

}

#endif
