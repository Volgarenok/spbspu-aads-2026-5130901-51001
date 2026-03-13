#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace shevchenko
{
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
