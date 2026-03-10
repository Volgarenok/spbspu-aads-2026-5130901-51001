#ifndef KRIVOSHAPOV_LIST_HPP
#define KRIVOSHAPOV_LIST_HPP

#include "iterator.hpp"
#include "node.hpp"

namespace krivoshapov
{
  template <class T>
  class List
  {
  public:
    using iterator = LIter<T>;
    using const_iterator = LCIter<T>;

    List() : head_(nullptr),
             tail_(nullptr),
             size_(0)
    {
    }

    List(const List &other) : head_(nullptr),
                              tail_(nullptr),
                              size_(0)
    {
      try
      {
        for (auto it = other.cbegin(); it != other.cend(); ++it)
        {
          pushBack(*it);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }
    List(List &&other) noexcept : head_(other.head_),
                                  tail_(other.tail_),
                                  size_(other.size_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List &operator=(const List &other)
    {
      if (this != &other)
      {
        List tmp(other);
        swapWith(tmp);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other)
      {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }
    List(List &&other) noexcept : head_(other.head_),
                                  tail_(other.tail_),
                                  size_(other.size_)
    {
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }

    ~List()
    {
      clear();
    }

    List &operator=(const List &other)
    {
      if (this != &other)
      {
        List tmp(other);
        swapWith(tmp);
      }
      return *this;
    }

    List &operator=(List &&other) noexcept
    {
      if (this != &other)
      {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;
        other.head_ = nullptr;
        other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    iterator begin()
    {
      return iterator(head_);
    }

    iterator end()
    {
      return iterator(nullptr);
    }

    const_iterator begin() const
    {
      return const_iterator(head_);
    }

    const_iterator end() const
    {
      return const_iterator(nullptr);
    }

    const_iterator cbegin() const
    {
      return const_iterator(head_);
    }

    const_iterator cend() const
    {
      return const_iterator(nullptr);
    }

    T &front()
    {
      return head_->value_;
    }

    const T &front() const
    {
      return head_->value_;
    }

    T &back()
    {
      return tail_->value_;
    }

    const T &back() const
    {
      return tail_->value_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    int size() const
    {
      return size_;
    }

    void pushFront(const T &value)
    {
      Node *node = new Node(value);
      linkFront(node);
    }

    void pushFront(T &&value)
    {
      Node *node = new Node(static_cast<T &&>(value));
      linkFront(node);
    }

    void pushBack(const T &value)
    {
      Node *node = new Node(value);
      linkBack(node);
    }

    void pushBack(T &&value)
    {
      Node *node = new Node(static_cast<T &&>(value));
      linkBack(node);
    }
  };
}
#endif
