#ifndef KRIVOSHAPOV_ITERATOR_HPP
#define KRIVOSHAPOV_ITERATOR_HPP

#include "node.hpp"

namespace krivoshapov
{

  template <class T>
  class List;

  template <class T>
  class LCIter;

  template <class T>
  class LIter
  {
    friend class List<T>;
    friend class LCIter<T>;

  public:
    LIter() : node_(nullptr)
    {
    }

    T &operator*() const
    {
      return node_->value_;
    }

    T *operator->() const
    {
      return &(node_->value_);
    }

    LIter &operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter operator++(int)
    {
      LIter tmp(*this);
      node_ = node_->next_;
      return tmp;
    }

    LIter &operator--()
    {
      node_ = node_->prev_;
      return *this;
    }

    LIter operator--(int)
    {
      LIter tmp(*this);
      node_ = node_->prev_;
      return tmp;
    }

    bool operator==(const LIter &rhs) const
    {
      return node_ == rhs.node_;
    }

    bool operator!=(const LIter &rhs) const
    {
      return node_ != rhs.node_;
    }

  private:
    using Node = detail::Node<T>;

    explicit LIter(Node *node) : node_(node)
    {
    }

    Node *node_;
  };
}

#endif
