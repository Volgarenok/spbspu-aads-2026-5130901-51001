#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <utility>

namespace karpenko
{
  namespace detail
  {
    struct NodeBase
    {
      NodeBase* next_;
      explicit NodeBase(NodeBase* nxt = nullptr) noexcept : next_(nxt) {}
    };

    template< typename T >
    struct Node final : NodeBase
    {
      T data_;

      explicit Node(NodeBase* nxt, const T& value)
        : NodeBase(nxt), data_(value)
      {}

      explicit Node(NodeBase* nxt, T&& value)
        : NodeBase(nxt), data_(std::move(value))
      {}
    };
  }

  template< typename T > class List;
  template< typename T > class LIter;
  template< typename T > class LCIter;

  template< typename T >
  class LIter
  {
  public:
    LIter() noexcept : ptr_(nullptr) {}
    explicit LIter(detail::NodeBase* p) noexcept : ptr_(p) {}

    T& operator*() const noexcept
    {
      detail::Node< T >* node = static_cast< detail::Node< T >* >(ptr_);
      return node->data_;
    }

    T* operator->() const noexcept
    {
      detail::Node< T >* node = static_cast< detail::Node< T >* >(ptr_);
      return &(node->data_);
    }

    LIter& operator++() noexcept
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LIter operator++(int) noexcept
    {
      LIter tmp = *this;
      ptr_ = ptr_->next_;
      return tmp;
    }

    bool operator==(const LIter& other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LIter& other) const noexcept
    {
      return !(*this == other);
    }

    detail::NodeBase* getPtr() const noexcept
    {
      return ptr_;
    }

  private:
    detail::NodeBase* ptr_;
    friend class List< T >;
    friend class LCIter< T >;
  };

  template< typename T >
  class LCIter
  {
  public:
    LCIter() noexcept : ptr_(nullptr) {}
    explicit LCIter(const detail::NodeBase* p) noexcept : ptr_(p) {}
    LCIter(const LIter< T >& it) noexcept : ptr_(it.getPtr()) {}

    const T& operator*() const noexcept
    {
      const detail::Node< T >* node = static_cast< const detail::Node< T >* >(ptr_);
      return node->data_;
    }

    const T* operator->() const noexcept
    {
      const detail::Node< T >* node = static_cast< const detail::Node< T >* >(ptr_);
      return &(node->data_);
    }

    LCIter& operator++() noexcept
    {
      ptr_ = ptr_->next_;
      return *this;
    }

    LCIter operator++(int) noexcept
    {
      LCIter tmp = *this;
      ptr_ = ptr_->next_;
      return tmp;
    }

    bool operator==(const LCIter& other) const noexcept
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const LCIter& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    const detail::NodeBase* ptr_;
    friend class List< T >;
  };

  template< typename T >
  class List
  {
  public:
    using iterator = LIter< T >;
    using const_iterator = LCIter< T >;
    using size_type = std::size_t;

    List()
      : head_(new detail::NodeBase), tail_(head_)
    {
      head_->next_ = head_;
    }

    ~List()
    {
      clear();
      delete head_;
    }

    List(const List& other) : List()
    {
      try
      {
        for (const_iterator it = other.cbegin(); it != other.cend(); ++it)
          pushBack(*it);
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    List(List&& other) noexcept
      : head_(other.head_), tail_(other.tail_)
    {
      other.head_ = new detail::NodeBase;
      other.head_->next_ = other.head_;
      other.tail_ = other.head_;
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
        delete head_;
        head_ = other.head_;
        tail_ = other.tail_;
        other.head_ = new detail::NodeBase;
        other.head_->next_ = other.head_;
        other.tail_ = other.head_;
      }
      return *this;
    }

    iterator begin() noexcept
    {
      return iterator(head_->next_);
    }

    const_iterator begin() const noexcept
    {
      return const_iterator(head_->next_);
    }

    iterator end() noexcept
    {
      return iterator(head_);
    }

    const_iterator end() const noexcept
    {
      return const_iterator(head_);
    }

    const_iterator cbegin() const noexcept
    {
      return const_iterator(head_->next_);
    }

    const_iterator cend() const noexcept
    {
      return const_iterator(head_);
    }

    bool empty() const noexcept
    {
      return head_->next_ == head_;
    }

    size_type size() const noexcept
    {
      size_type count = 0;
      for (const_iterator it = cbegin(); it != cend(); ++it)
        ++count;
      return count;
    }

    T& front()
    {
      if (empty())
        throw std::out_of_range("List is empty");
      return *begin();
    }

    const T& front() const
    {
      if (empty())
        throw std::out_of_range("List is empty");
      return *begin();
    }

    T& back()
    {
      if (empty())
        throw std::out_of_range("List is empty");
      return *iterator(tail_);
    }

    const T& back() const
    {
      if (empty())
        throw std::out_of_range("List is empty");
      return *const_iterator(tail_);
    }

    void pushFront(const T& value)
    {
      insertAfter(iterator(head_), value);
    }

    void pushFront(T&& value)
    {
      insertAfter(iterator(head_), std::move(value));
    }

    void pushBack(const T& value)
    {
      if (empty())
        pushFront(value);
      else
        insertAfter(iterator(tail_), value);
    }

    void pushBack(T&& value)
    {
      if (empty())
        pushFront(std::move(value));
      else
        insertAfter(iterator(tail_), std::move(value));
    }

    void popFront()
    {
      if (empty())
        throw std::out_of_range("List is empty");
      detail::NodeBase* old = head_->next_;
      head_->next_ = old->next_;
      if (tail_ == old)
        tail_ = head_;
      delete static_cast< detail::Node< T >* >(old);
    }

    void popBack()
    {
      if (empty())
        throw std::out_of_range("List is empty");
      if (head_->next_ == tail_)
      {
        popFront();
        return;
      }
      detail::NodeBase* prev = head_->next_;
      while (prev->next_ != tail_)
        prev = prev->next_;
      delete static_cast< detail::Node< T >* >(tail_);
      tail_ = prev;
      tail_->next_ = head_;
    }

    iterator insertAfter(iterator pos, const T& value)
    {
      detail::Node< T >* new_node =
          new detail::Node< T >(pos.getPtr()->next_, value);
      pos.getPtr()->next_ = new_node;
      if (tail_ == pos.getPtr())
        tail_ = new_node;
      return iterator(new_node);
    }

    iterator insertAfter(iterator pos, T&& value)
    {
      detail::Node< T >* new_node =
          new detail::Node< T >(pos.getPtr()->next_, std::move(value));
      pos.getPtr()->next_ = new_node;
      if (tail_ == pos.getPtr())
        tail_ = new_node;
      return iterator(new_node);
    }

    iterator eraseAfter(iterator pos)
    {
      if (pos.getPtr() == head_ || pos.getPtr()->next_ == head_)
        throw std::out_of_range("Invalid position for eraseAfter");
      detail::NodeBase* to_delete = pos.getPtr()->next_;
      pos.getPtr()->next_ = to_delete->next_;
      if (tail_ == to_delete)
        tail_ = pos.getPtr();
      delete static_cast< detail::Node< T >* >(to_delete);
      return iterator(pos.getPtr()->next_);
    }

    void clear() noexcept
    {
      while (!empty())
        popFront();
    }

    void swap(List& other) noexcept
    {
      detail::NodeBase* tmp_head = head_;
      detail::NodeBase* tmp_tail = tail_;
      head_ = other.head_;
      tail_ = other.tail_;
      other.head_ = tmp_head;
      other.tail_ = tmp_tail;
    }

  private:
    detail::NodeBase* head_;
    detail::NodeBase* tail_;
  };

  template< typename T >
  void swap(List< T >& lhs, List< T >& rhs) noexcept
  {
    lhs.swap(rhs);
  }

}

#endif
