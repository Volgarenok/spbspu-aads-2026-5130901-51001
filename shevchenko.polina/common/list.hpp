#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace shevchenko
{
template< class T >
class List;

template< class T >
struct Node
{
  T data;
  Node< T >* next;
  Node< T >* prev;

  explicit Node(const T& value) :
  data(value),
  next(nullptr),
  prev(nullptr)
  {
  }
};

template< class T >
class LCIter
{
  friend class List< T >;

public:
  LCIter() :
  ptr_(nullptr)
  {
  }

  LCIter(const LCIter&) = default;

  LCIter< T >& operator++()
  {
    if (ptr_ != nullptr)
    {
      ptr_ = ptr_->next;
    }
    return *this;
  }

  LCIter< T > operator++(int)
  {
    LCIter< T > tmp = *this;
    ++(*this);
    return tmp;
  }

  LCIter< T >& operator--()
  {
    if (ptr_ != nullptr)
    {
      ptr_ = ptr_->prev;
    }
    return *this;
  }

  LCIter< T > operator--(int)
  {
    LCIter< T > tmp = *this;
    --(*this);
    return tmp;
  }

  const T& operator*() const
  {
    return ptr_->data;
  }

  bool operator==(const LCIter< T >& other) const
  {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const LCIter< T >& other) const
  {
    return ptr_ != other.ptr_;
  }
private:
  explicit LCIter(const Node< T >* ptr) :
  ptr_(ptr)
  {
  }

  const Node< T >* ptr_;
};

template< class T >
class LIter
{
  friend class List< T >;

public:
  LIter() :
  ptr_(nullptr)
  {
  }

  LIter(const LIter&) = default;

  operator LCIter< T >() const
  {
    return LCIter< T >(ptr_);
  }

  LIter< T >& operator++()
  {
    if (ptr_ != nullptr)
    {
      ptr_ = ptr_->next;
    }
    return *this;
  }

  LIter< T > operator++(int)
  {
    LIter< T > tmp = *this;
    ++(*this);
    return tmp;
  }

  LIter< T >& operator--()
  {
    if (ptr_ != nullptr)
    {
      ptr_ = ptr_->prev;
    }
    return *this;
  }

  LIter< T > operator--(int)
  {
    LIter< T > tmp = *this;
    --(*this);
    return tmp;
  }

  T& operator*() const
  {
    return ptr_->data;
  }

  bool operator==(const LIter< T >& other) const
  {
    return ptr_ == other.ptr_;
  }

  bool operator!=(const LIter< T >& other) const
  {
    return ptr_ != other.ptr_;
  }

private:
  explicit LIter(Node< T >* ptr) :
  ptr_(ptr)
  {
  }

  Node< T >* ptr_;
};

template< class T >
class List
{
public:
  using iterator = LIter< T >;
  using const_iterator = LCIter< T >;

  List() :
  head_(nullptr),
  tail_(nullptr),
  size_(0)
  {
  }

  List(const List& other) :
  head_(nullptr),
  tail_(nullptr),
  size_(0)
  {
    try
    {
      for (Node< T >* curr = other.head_; curr != nullptr; curr = curr->next)
      {
        this->pushBack(curr->data);
      }
    }
    catch (...)
    {
      this->clear();
      throw;
    }
  }

  List(List&& other) noexcept :
  head_(other.head_),
  tail_(other.tail_),
  size_(other.size_)
  {
    other.head_ = nullptr;
    other.tail_ = nullptr;
    other.size_ = 0;
  }

  ~List()
  {
    this->clear();
  }

  List< T >& operator=(const List& other)
  {
    if (this != &other)
    {
      List< T > tmp(other);
      this->swap(tmp);
    }
    return *this;
  }

  List< T >& operator=(List&& other) noexcept
  {
    if (this != &other)
    {
      this->clear();
      head_ = other.head_;
      tail_ = other.tail_;
      size_ = other.size_;
      other.head_ = nullptr;
      other.tail_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  iterator begin() noexcept
  {
    return iterator(head_);
  }

  const_iterator begin() const noexcept
  {
    return const_iterator(head_);
  }

  const_iterator cbegin() const noexcept
  {
    return const_iterator(head_);
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

  T& front()
  {
    return head_->data;
  }

  const T& front() const
  {
    return head_->data;
  }

  T& back()
  {
    return tail_->data;
  }

  const T& back() const
  {
    return tail_->data;
  }

  void pushFront(const T& value)
  {
    Node< T >* new_node = new Node< T >(value);
    new_node->next = head_;

    if (head_ != nullptr)
    {
      head_->prev = new_node;
    }
    else
    {
      tail_ = new_node;
    }
    head_ = new_node;
    ++size_;
  }

  void pushBack(const T& value)
  {
    Node< T >* new_node = new Node< T >(value);
    new_node->prev = tail_;

    if (tail_ != nullptr)
    {
      tail_->next = new_node;
    }
    else
    {
      head_ = new_node;
    }
    tail_ = new_node;
    ++size_;
  }

  void popFront()
  {
    if (this->empty())
    {
      return;
    }

    Node< T >* old_head = head_;
    head_ = head_->next;

    if (head_ != nullptr)
    {
      head_->prev = nullptr;
    }
    else
    {
      tail_ = nullptr;
    }

    delete old_head;
    --size_;
  }

  void popBack()
  {
    if (this->empty())
    {
      return;
    }

    Node< T >* old_tail = tail_;
    tail_ = tail_->prev;

    if (tail_ != nullptr)
    {
      tail_->next = nullptr;
    }
    else
    {
      head_ = nullptr;
    }

    delete old_tail;
    --size_;
  }

  iterator insertAfter(iterator pos, const T& value)
  {
    if (pos == this->end())
    {
      this->pushBack(value);
      return iterator(tail_);
    }

    Node< T >* current = pos.ptr_;
    Node< T >* new_node = new Node< T >(value);

    new_node->next = current->next;
    new_node->prev = current;

    if (current->next != nullptr)
    {
      current->next->prev = new_node;
    }
    else
    {
      tail_ = new_node;
    }

    current->next = new_node;
    ++size_;

    return iterator(new_node);
  }

  iterator erase(iterator pos)
  {
    if (pos == this->end())
    {
      return this->end();
    }

    Node< T >* to_delete = pos.ptr_;
    iterator next_it(to_delete->next);

    if (to_delete->prev != nullptr)
    {
      to_delete->prev->next = to_delete->next;
    }
    else
    {
      head_ = to_delete->next;
    }

    if (to_delete->next != nullptr)
    {
      to_delete->next->prev = to_delete->prev;
    }
    else
    {
      tail_ = to_delete->prev;
    }

    delete to_delete;
    --size_;

    return next_it;
  }

  void clear()
  {
    while (head_ != nullptr)
    {
      Node< T >* next = head_->next;
      delete head_;
      head_ = next;
    }
    tail_ = nullptr;
    size_ = 0;
  }

  size_t size() const
  {
    return size_;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  void swap(List& other) noexcept
  {
    Node< T >* tmp_head = other.head_;
    Node< T >* tmp_tail = other.tail_;
    size_t tmp_size = other.size_;

    other.head_ = head_;
    other.tail_ = tail_;
    other.size_ = size_;

    head_ = tmp_head;
    tail_ = tmp_tail;
    size_ = tmp_size;
  }

private:
  Node< T >* head_;
  Node< T >* tail_;
  size_t size_;
};
}

#endif
