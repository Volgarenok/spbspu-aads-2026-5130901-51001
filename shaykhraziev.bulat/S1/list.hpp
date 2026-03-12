#ifndef LIST_HPP
#define LIST_HPP

#include "node.hpp"

namespace shaykhraziev
{
  template< class T >
  class LCIter;

  template< class T >
  class LIter
  {
  public:
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    explicit LIter(NodeBase* node) :
      node_(node)
    {}

    reference operator*() const
    {
      return static_cast< Node< T >* >(node_)->data_;
    }

    pointer operator->() const
    {
      return &(static_cast< Node< T >* >(node_)->data_);
    }

    LIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LIter operator++(int)
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
      return node_ != other.node_;
    }

    operator LCIter< T >() const;

    NodeBase* node_;
  };

  template< class T >
  class LCIter
  {
  public:
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    explicit LCIter(const NodeBase* node) :
      node_(node)
    {}

    reference operator*() const
    {
      return static_cast< const Node< T >* >(node_)->data_;
    }

    pointer operator->() const
    {
      return &(static_cast< const Node< T >* >(node_)->data_);
    }

    LCIter& operator++()
    {
      node_ = node_->next_;
      return *this;
    }

    LCIter operator++(int)
    {
      LCIter tmp = *this;
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

    const NodeBase* node_;
  };

  template< class T >
  LIter< T >::operator LCIter< T >() const
  {
    return LCIter< T >(node_);
  }

  template< class T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;

    List() :
      fake_()
    {}

    ~List()
    {
      clear();
    }

    List(const List& other) :
      fake_()
    {
      copyFrom(other);
    }

    List(List&& other) noexcept :
      fake_()
    {
      swapWith(other);
    }

    List& operator=(List other)
    {
      swapWith(other);
      return *this;
    }

    void swap(List& other) noexcept
    {
      swapWith(other);
    }

    iterator before_begin() noexcept
    {
      return iterator(&fake_);
    }

    const_iterator before_begin() const noexcept
    {
      return const_iterator(&fake_);
    }

    const_iterator cbefore_begin() const noexcept
    {
      return const_iterator(&fake_);
    }

    iterator begin() noexcept
    {
      return iterator(fake_.next_);
    }

    const_iterator begin() const noexcept
    {
      return const_iterator(fake_.next_);
    }

    const_iterator cbegin() const noexcept
    {
      return const_iterator(fake_.next_);
    }

    iterator end() noexcept
    {
      return iterator(nullptr);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(nullptr);
    }

    const_iterator cend() const noexcept
    {
      return const_iterator(nullptr);
    }

    bool empty() const noexcept
    {
      return fake_.next_ == nullptr;
    }

    T& front()
    {
      return static_cast< Node< T >* >(fake_.next_)->data_;
    }

    const T& front() const
    {
      return static_cast< const Node< T >* >(fake_.next_)->data_;
    }

    void push_front(const T& val)
    {
      Node< T >* newNode = new Node< T >(val);
      newNode->next_ = fake_.next_;
      fake_.next_ = newNode;
    }

    void push_front(T&& val)
    {
      Node< T >* newNode = new Node< T >(std::move(val));
      newNode->next_ = fake_.next_;
      fake_.next_ = newNode;
    }

    void pop_front()
    {
      NodeBase* toDelete = fake_.next_;
      fake_.next_ = toDelete->next_;
      delete static_cast< Node< T >* >(toDelete);
    }

    void clear() noexcept
    {
      while (!empty())
      {
        pop_front();
      }
    }

    iterator insert_after(iterator pos, const T& val)
    {
      Node< T >* newNode = new Node< T >(val);
      newNode->next_ = pos.node_->next_;
      pos.node_->next_ = newNode;
      return iterator(newNode);
    }

    iterator insert_after(iterator pos, T&& val)
    {
      Node< T >* newNode = new Node< T >(std::move(val));
      newNode->next_ = pos.node_->next_;
      pos.node_->next_ = newNode;
      return iterator(newNode);
    }

    iterator erase_after(iterator pos)
    {
      NodeBase* toDelete = pos.node_->next_;
      pos.node_->next_ = toDelete->next_;
      delete static_cast< Node< T >* >(toDelete);
      return iterator(pos.node_->next_);
    }

  private:
    NodeBase fake_;

    void swapWith(List& other) noexcept
    {
      NodeBase* tmp = fake_.next_;
      fake_.next_ = other.fake_.next_;
      other.fake_.next_ = tmp;
    }

    void copyFrom(const List& other)
    {
      iterator tail = before_begin();
      for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
      {
        tail = insert_after(tail, *it);
      }
    }
  };
}

#endif

