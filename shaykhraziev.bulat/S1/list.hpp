#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include <utility>

namespace shaykhraziev
{
  template< class T >
  class List;

  template< class T >
  class LIter
  {
  public:
    LIter(NodeBase* node = nullptr, const NodeBase* first = nullptr):
      node_(node),
      first_(first)
    {}

    T& operator*() const
    {
      return static_cast< Node< T >* >(node_)->data_;
    }

    T* operator->() const
    {
      return &(static_cast< Node< T >* >(node_)->data_);
    }

    LIter& operator++()
    {
      if (!node_)
      {
        return *this;
      }
      if (node_->next_ == first_)
      {
        node_ = nullptr;
      }
      else
      {
        node_ = node_->next_;
      }
      return *this;
    }

    bool operator==(const LIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LIter& other) const
    {
      return node_ != other.node_;
    }

  private:
    NodeBase* node_;
    const NodeBase* first_;

    template< class U >
    friend class List;
  };

  template< class T >
  class LCIter
  {
  public:
    LCIter(const NodeBase* node = nullptr, const NodeBase* first = nullptr):
      node_(node),
      first_(first)
    {}

    const T& operator*() const
    {
      return static_cast< const Node< T >* >(node_)->data_;
    }

    const T* operator->() const
    {
      return &(static_cast< const Node< T >* >(node_)->data_);
    }

    LCIter& operator++()
    {
      if (!node_)
      {
        return *this;
      }
      if (node_->next_ == first_)
      {
        node_ = nullptr;
      }
      else
      {
        node_ = node_->next_;
      }
      return *this;
    }

    bool operator==(const LCIter& other) const
    {
      return node_ == other.node_;
    }

    bool operator!=(const LCIter& other) const
    {
      return node_ != other.node_;
    }

  private:
    const NodeBase* node_;
    const NodeBase* first_;

    template< class U >
    friend class List;
  };

  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List():
      tail_(nullptr)
    {}

    ~List()
    {
      clear();
    }

    List(const List& other):
      tail_(nullptr)
    {
      copyFrom(other);
    }

    List(List&& other) noexcept:
      tail_(other.tail_)
    {
      other.tail_ = nullptr;
    }

    List& operator=(const List& other)
    {
      if (this != &other)
      {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other)
      {
        clear();
        tail_ = other.tail_;
        other.tail_ = nullptr;
      }
      return *this;
    }

    iterator begin() noexcept
    {
      NodeBase* first = head();
      return iterator(first, first);
    }

    iterator end() noexcept
    {
      return iterator(nullptr, head());
    }

    const_iterator begin() const noexcept
    {
      const NodeBase* first = head();
      return const_iterator(first, first);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(nullptr, head());
    }

    bool empty() const noexcept
    {
      return tail_ == nullptr;
    }

    void push_back(const T& value)
    {
      Node< T >* node = new Node< T >(value);
      pushBackNode(node);
    }

    void push_back(T&& value)
    {
      Node< T >* node = new Node< T >(std::move(value));
      pushBackNode(node);
    }

    void clear() noexcept
    {
      if (!tail_)
      {
        return;
      }

      NodeBase* first = tail_->next_;
      tail_->next_ = nullptr;

      while (first)
      {
        NodeBase* next = first->next_;
        delete static_cast< Node< T >* >(first);
        first = next;
      }

      tail_ = nullptr;
    }

  private:
    NodeBase* tail_;

    NodeBase* head() noexcept
    {
      if (!tail_)
      {
        return nullptr;
      }
      return tail_->next_;
    }

    const NodeBase* head() const noexcept
    {
      if (!tail_)
      {
        return nullptr;
      }
      return tail_->next_;
    }

    void pushBackNode(NodeBase* node)
    {
      if (!tail_)
      {
        node->next_ = node;
        tail_ = node;
        return;
      }

      node->next_ = tail_->next_;
      tail_->next_ = node;
      tail_ = node;
    }

    void swap(List& other) noexcept
    {
      NodeBase* tmp = tail_;
      tail_ = other.tail_;
      other.tail_ = tmp;
    }

    void copyFrom(const List& other)
    {
      for (auto it = other.begin(); it != other.end(); ++it)
      {
        push_back(*it);
      }
    }
  };
}

#endif