#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"

namespace vishnyakov
{
  template< class T > class List;

  template< class T >
  class LIter
  {
    friend class List< T >;

    Node* node_;

  public:
    LIter(Node* node = nullptr):
      node_(node)
    {}

    T* operator*() const
    {
      return node_->data_;
    }

    T& operator->() const
    {
      return *node_->data_;
    }

    LIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter* operator++(int)
    {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter& other) const
    {
      return !(node_ == other);
    }
  };

  template< class T >
  class LCIter
  {
    friend class List< T >;
    Node* node_;

  public:
    LIter(const Node* node = nullptr):
      node_(node)
    {}

    LIter(const LIter< T > it):
      node_(it.node_)
    {}

    T* operator*() const
    {
      return node_->data_;
    }

    T& operator->() const
    {
      return *node_->data_;
    }

    LIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter* operator++(int)
    {
      LIter tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const LIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter& other) const
    {
      return !(node_ == other);
    }
  };

  template< class T >
  class List
  {
    Node* pseudoknot_;
    size_t size_;

    List():
      pseudoknot_(new Node()),
      size_(0)
    {
      pseudoknot_->next_ = pseudoknot_;
    }


  };
}

#endif