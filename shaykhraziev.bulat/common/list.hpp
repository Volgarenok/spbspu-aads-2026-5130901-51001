#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"
#include <cstddef>
#include <iterator>
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

    LCIter(const LIter< T >& other):
      node_(other.node_),
      first_(other.first_)
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

    LCIter operator++(int)
    {
      LCIter tmp(*this);
      ++(*this);
      return tmp;
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
      tail_(nullptr),
      size_(0)
    {}

    ~List()
    {
      clear();
    }

    List(const List& other):
      tail_(nullptr),
      size_(0)
    {
      copyFrom(other);
    }

    List(List&& other) noexcept:
      tail_(other.tail_),
      size_(other.size_)
    {
      other.tail_ = nullptr;
      other.size_ = 0;
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
        size_ = other.size_;
        other.tail_ = nullptr;
        other.size_ = 0;
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

    const_iterator cbegin() const noexcept
    {
      return begin();
    }

    const_iterator cend() const noexcept
    {
      return end();
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    T& front() noexcept
    {
      return static_cast< Node< T >* >(tail_->next_)->data_;
    }

    const T& front() const noexcept
    {
      return static_cast< const Node< T >* >(tail_->next_)->data_;
    }

    T& back() noexcept
    {
      return static_cast< Node< T >* >(tail_)->data_;
    }

    const T& back() const noexcept
    {
      return static_cast< const Node< T >* >(tail_)->data_;
    }

    void pushFront(const T& value)
    {
      Node< T >* node = new Node< T >(value);
      linkFront(node);
    }

    void pushFront(T&& value)
    {
      Node< T >* node = new Node< T >(std::move(value));
      linkFront(node);
    }

    void pushBack(const T& value)
    {
      Node< T >* node = new Node< T >(value);
      linkBack(node);
    }

    void pushBack(T&& value)
    {
      Node< T >* node = new Node< T >(std::move(value));
      linkBack(node);
    }

    void popFront() noexcept
    {
      NodeBase* first = tail_->next_;
      if (first == tail_)
      {
        delete static_cast< Node< T >* >(first);
        tail_ = nullptr;
        size_ = 0;
        return;
      }

      tail_->next_ = first->next_;
      delete static_cast< Node< T >* >(first);
      --size_;
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
      size_ = 0;
    }

    void swap(List& other) noexcept
    {
      NodeBase* tmpTail = tail_;
      tail_ = other.tail_;
      other.tail_ = tmpTail;

      std::size_t tmpSize = size_;
      size_ = other.size_;
      other.size_ = tmpSize;
    }

  private:
    NodeBase* tail_;
    std::size_t size_;

    NodeBase* head() noexcept
    {
      return tail_ ? tail_->next_ : nullptr;
    }

    const NodeBase* head() const noexcept
    {
      return tail_ ? tail_->next_ : nullptr;
    }

    void linkFront(NodeBase* node) noexcept
    {
      if (!tail_)
      {
        node->next_ = node;
        tail_ = node;
      }
      else
      {
        node->next_ = tail_->next_;
        tail_->next_ = node;
      }
      ++size_;
    }

    void linkBack(NodeBase* node) noexcept
    {
      if (!tail_)
      {
        node->next_ = node;
        tail_ = node;
      }
      else
      {
        node->next_ = tail_->next_;
        tail_->next_ = node;
        tail_ = node;
      }
      ++size_;
    }

    void copyFrom(const List& other)
    {
      for (const_iterator it = other.begin(); it != other.end(); ++it)
      {
        pushBack(*it);
      }
    }
  };
}

#endif
