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

  template< class T >
  LIter< T >::LIter():
    node_(nullptr)
  {}

  template< class T >
  LIter< T >::LIter(detail::NodeBase * node):
    node_(node)
  {}

  template< class T >
  T & LIter< T >::operator*() const
  {
    return static_cast< detail::Node< T > * >(node_)->data;
  }

  template< class T >
  T * LIter< T >::operator->() const
  {
    return &(static_cast< detail::Node< T > * >(node_)->data);
  }

  template< class T >
  LIter< T > & LIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LIter< T > LIter< T >::operator++(int)
  {
    LIter tmp(*this);
    node_ = node_->next;
    return tmp;
  }

  template< class T >
  bool LIter< T >::operator==(const LIter & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LIter< T >::operator!=(const LIter & rhs) const
  {
    return node_ != rhs.node_;
  }

  template< class T >
  LCIter< T >::LCIter():
    node_(nullptr)
  {}

  template< class T >
  LCIter< T >::LCIter(const detail::NodeBase * node):
    node_(node)
  {}

  template< class T >
  LCIter< T >::LCIter(const LIter< T > & other):
    node_(other.node_)
  {}

  template< class T >
  const T & LCIter< T >::operator*() const
  {
    return static_cast< const detail::Node< T > * >(node_)->data;
  }

  template< class T >
  const T * LCIter< T >::operator->() const
  {
    return &(static_cast< const detail::Node< T > * >(node_)->data);
  }

  template< class T >
  LCIter< T > & LCIter< T >::operator++()
  {
    node_ = node_->next;
    return *this;
  }

  template< class T >
  LCIter< T > LCIter< T >::operator++(int)
  {
    LCIter tmp(*this);
    node_ = node_->next;
    return tmp;
  }

  template< class T >
  bool LCIter< T >::operator==(const LCIter & rhs) const
  {
    return node_ == rhs.node_;
  }

  template< class T >
  bool LCIter< T >::operator!=(const LCIter & rhs) const
  {
    return node_ != rhs.node_;
  }
}

#endif
