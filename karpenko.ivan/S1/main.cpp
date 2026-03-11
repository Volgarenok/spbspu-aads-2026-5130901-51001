#include <iostream>
#include <string>
#include <utility>
#include <cassert>
#include <cstddef>
#include <limits>

namespace karpenko
{
  namespace detail
  {
    struct NodeBase
    {
      NodeBase* next;
      explicit NodeBase(NodeBase* nxt = nullptr) noexcept : next(nxt) {}
    };

    template< typename T >
    struct Node final : NodeBase
    {
      T data;
      explicit Node(NodeBase* nxt, const T& value) : NodeBase(nxt), data(value) {}
      explicit Node(NodeBase* nxt, T&& value) : NodeBase(nxt), data(std::move(value)) {}
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
      detail::Node< T >* node_ptr = static_cast< detail::Node< T >* >(ptr_);
      return node_ptr->data;
    }
    T* operator->() const noexcept
    {
      detail::Node< T >* node_ptr = static_cast< detail::Node< T >* >(ptr_);
      return &(node_ptr->data);
    }
    LIter& operator++() noexcept
    {
      ptr_ = ptr_->next;
      return *this;
    }
    LIter operator++(int) noexcept
    {
      LIter tmp = *this;
      ptr_ = ptr_->next;
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
    detail::NodeBase* get_ptr() const noexcept
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
    LCIter(const LIter< T >& it) noexcept : ptr_(it.get_ptr()) {}
    const T& operator*() const noexcept
    {
      const detail::Node< T >* node_ptr = static_cast< const detail::Node< T >* >(ptr_);
      return node_ptr->data;
    }
    const T* operator->() const noexcept
    {
      const detail::Node< T >* node_ptr = static_cast< const detail::Node< T >* >(ptr_);
      return &(node_ptr->data);
    }
    LCIter& operator++() noexcept
    {
      ptr_ = ptr_->next;
      return *this;
    }
    LCIter operator++(int) noexcept
    {
      LCIter tmp = *this;
      ptr_ = ptr_->next;
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

    List() : head_(new detail::NodeBase), tail_(head_)
    {
      head_->next = head_;
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
        for (const_iterator it = other.begin(); it != other.end(); ++it)
        {
          push_back(*it);
        }
      }
      catch (...)
      {
        clear();
        throw;
      }
    }

    List(List&& other) noexcept : head_(other.head_), tail_(other.tail_)
    {
      other.head_ = new detail::NodeBase;
      other.head_->next = other.head_;
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
        other.head_->next = other.head_;
        other.tail_ = other.head_;
      }
      return *this;
    }

    iterator begin() noexcept
    {
      return iterator(head_->next);
    }
    const_iterator begin() const noexcept
    {
      return const_iterator(head_->next);
    }
    iterator end() noexcept
    {
      return iterator(head_);
    }
    const_iterator end() const noexcept
    {
      return const_iterator(head_);
    }

    bool empty() const noexcept
    {
      return head_->next == head_;
    }

    T& front()
    {
      assert(!empty());
      return *begin();
    }

    const T& front() const
    {
      assert(!empty());
      return *begin();
    }

    T& back()
    {
      assert(!empty());
      return *iterator(tail_);
    }

    const T& back() const
    {
      assert(!empty());
      return *const_iterator(tail_);
    }

    size_type size() const noexcept
    {
      size_type count = 0;
      for (const_iterator it = begin(); it != end(); ++it)
      {
        ++count;
      }
      return count;
    }
  };
}

int main()
{

}
