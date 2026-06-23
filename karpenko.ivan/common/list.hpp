#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
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

      template< typename... Args >
      explicit Node(NodeBase* nxt, Args&&... args)
        : NodeBase(nxt), data_(std::forward<Args>(args)...)
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

    template< typename... Args >
    iterator emplaceFront(Args&&... args)
    {
      auto* new_node = new detail::Node< T >(head_->next_,
                                             std::forward<Args>(args)...);
      head_->next_ = new_node;
      if (tail_ == head_)
        tail_ = new_node;
      return iterator(new_node);
    }

    template< typename... Args >
    iterator emplaceBack(Args&&... args)
    {
      if (empty())
        return emplaceFront(std::forward<Args>(args)...);
      auto* new_node = new detail::Node< T >(tail_->next_,
                                             std::forward<Args>(args)...);
      tail_->next_ = new_node;
      tail_ = new_node;
      return iterator(new_node);
    }

    template< typename... Args >
    iterator emplaceAfter(iterator pos, Args&&... args)
    {
      auto* new_node = new detail::Node< T >(pos.getPtr()->next_,
                                             std::forward<Args>(args)...);
      pos.getPtr()->next_ = new_node;
      if (tail_ == pos.getPtr())
        tail_ = new_node;
      return iterator(new_node);
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


    void spliceAfter(iterator pos, List& other)
    {
      if (other.empty())
        return;
      detail::NodeBase* first = other.head_->next_;
      detail::NodeBase* last = other.tail_;
      other.head_->next_ = other.head_;
      other.tail_ = other.head_;
      detail::NodeBase* pos_next = pos.getPtr()->next_;
      pos.getPtr()->next_ = first;
      last->next_ = pos_next;
      if (tail_ == pos.getPtr())
        tail_ = last;
    }

    void spliceAfter(iterator pos, List& other, iterator it)
    {
      if (it.getPtr() == other.head_ || it.getPtr()->next_ == other.head_)
        throw std::out_of_range("Invalid iterator for spliceAfter");
      detail::NodeBase* node = it.getPtr()->next_;
      it.getPtr()->next_ = node->next_;
      if (other.tail_ == node)
        other.tail_ = it.getPtr();
      detail::NodeBase* pos_next = pos.getPtr()->next_;
      pos.getPtr()->next_ = node;
      node->next_ = pos_next;
      if (tail_ == pos.getPtr())
        tail_ = node;
    }

    void spliceAfter(iterator pos, List& other, iterator first, iterator last)
    {
      if (first.getPtr() == last.getPtr())
        return;
      detail::NodeBase* start = first.getPtr()->next_;
      detail::NodeBase* end = last.getPtr();
      first.getPtr()->next_ = end->next_;
      if (other.tail_ == end)
        other.tail_ = first.getPtr();
      detail::NodeBase* pos_next = pos.getPtr()->next_;
      pos.getPtr()->next_ = start;
      end->next_ = pos_next;
      if (tail_ == pos.getPtr())
        tail_ = end;
    }

    void sort()
    {
      if (empty() || begin().getPtr()->next_ == head_)
        return;
      size_type sz = size();
      size_type half = sz / 2;
      iterator mid = begin();
      for (size_type i = 1; i < half; ++i)
        ++mid;
      List second;
      second.spliceAfter(second.begin(), *this, mid, end());
      sort();
      second.sort();
      merge(second);
    }

    void merge(List& other)
    {
      if (this == &other)
        return;
      iterator pos = begin();
      while (!other.empty())
      {
        if (pos == end())
        {
          spliceAfter(iterator(tail_), other);
          break;
        }
        if (other.front() < *pos)
        {
          iterator next = other.begin();
          ++next;
          spliceAfter(pos, other, other.begin());
          ++pos;
        }
        else
        {
          ++pos;
        }
      }
    }

    template< typename UnaryPredicate >
    iterator partition(UnaryPredicate p)
    {
      if (empty())
        return end();

      detail::NodeBase true_head;
      detail::NodeBase false_head;
      detail::NodeBase* true_tail = &true_head;
      detail::NodeBase* false_tail = &false_head;

      detail::NodeBase* curr = head_->next_;
      head_->next_ = head_;
      while (curr != head_)
      {
        detail::NodeBase* next = curr->next_;
        T& data = static_cast< detail::Node< T >* >(curr)->data_;
        if (p(data))
        {
          true_tail->next_ = curr;
          true_tail = curr;
        }
        else
        {
          false_tail->next_ = curr;
          false_tail = curr;
        }
        curr = next;
      }

      true_tail->next_ = false_head.next_;
      false_tail->next_ = head_;
      head_->next_ = true_head.next_;

      if (false_tail != &false_head)
        tail_ = false_tail;
      else if (true_tail != &true_head)
        tail_ = true_tail;
      else
        tail_ = head_;

      if (false_head.next_ == nullptr)
        return end();
      else
        return iterator(false_head.next_);
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

  template class List< int >;
  template class List< long long >;
  template class List< std::string >;
  template class List< char >;

}

#endif
