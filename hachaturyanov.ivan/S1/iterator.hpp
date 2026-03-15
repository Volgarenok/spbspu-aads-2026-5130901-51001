#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "node.hpp"

namespace hachaturyanov
{
  template< class T > class List;
  template< class T > class LCIter {
    friend class List< T >;
    explicit LCIter(const node< T >* n):
     node_(n)
    {}
    const node< T >* node_;
  public:
    const T& operator*() const;
    const T* operator->() const;
    LCIter& operator++() noexcept;
    LCIter& operator--() noexcept;
    bool operator==(const LCIter& other) const noexcept;
    bool operator!=(const LCIter& other) const noexcept;

  };
  template< class T > class LIter {
    friend class List< T >;
    explicit LIter(node< T >* n):
     node_(n)
    {}
  public:
    node< T >* node_;
    T& operator*() const;
    T* operator->() const;
    LIter& operator++() noexcept;
    LIter& operator--() noexcept;
    bool operator==(const LIter& other) const noexcept;
    bool operator!=(const LIter& other) const noexcept;
  };

  template< class T > const T& LCIter< T >::operator*() const {
    return node_->val_;
  }

  template< class T > const T* LCIter< T >::operator->() const {
    return &(node_->val_);
  }

  template< class T > LCIter< T >& LCIter< T >::operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  template< class T > LCIter< T >& LCIter< T >::operator--() noexcept {
    node_ = node_->prev_;
    return *this;
  }

  template< class T > bool LCIter< T >::operator==(const LCIter< T >& other) const noexcept {
    return node_ == other.node_;
  }

  template< class T > bool LCIter< T >::operator!=(const LCIter< T >& other) const noexcept {
    return node_ != other.node_;
  }

  template< class T > T& LIter< T >::operator*() const {
    return node_->val_;
  }

  template< class T > T* LIter< T >::operator->() const {
    return &(node_->val_);
  }

  template< class T > LIter< T >& LIter< T >::operator++() noexcept {
    node_ = node_->next_;
    return *this;
  }

  template< class T > LIter< T >& LIter< T >::operator--() noexcept {
    node_ = node_->prev_;
    return *this;
  }

  template< class T > bool LIter< T >::operator==(const LIter< T >& other) const noexcept {
    return node_ == other.node_;
  }

  template< class T > bool LIter< T >::operator!=(const LIter< T >& other) const noexcept {
    return node_ != other.node_;
  }

}

#endif
