#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include "iterator.hpp"

namespace hachaturyanov
{
  template< class T > class List {
    node< T >* head_;
    size_t size_;
  public:
    using iter = LIter< T >;
    using citer = LCIter< T >;

    List() noexcept;
    ~List();
    List(const List &other);

    size_t size() const noexcept;
    bool isEmpty() const noexcept;

    iter begin();
    iter end();
    citer begin() const;
    citer end() const;

    iter addFront(const T &val);
    iter addEnd(const T &val);

    iter insertBefore(iter pos, const T &val);
    iter insertAfter(iter pos, const T &val);

    void popFront() noexcept;
    void popEnd() noexcept;

    void erase(iter pos) noexcept;

    void clear() noexcept;
    void clear(iter from, iter to) noexcept;

    List& operator=(List &other);
    List& operator=(List &&other);
  };


  template< class T > List< T >::List() noexcept:
  head_(nullptr),
  size_(0)
  {}

  template< class T > List< T >::~List()
  {
    clear();
  }

  template< class T > List< T >::List(const List &other):
   head_(nullptr),
   size_(0)
  {
    if (!other.size_) {
      citer it = other.begin();
      addEnd(*it);
      ++it;
      for (; it != other.begin(); ++it) {
        addEnd(*it);
      }
    }
  }

  template< class T > size_t List< T >::size() const noexcept
  {
    return size_;
  }

  template< class T > bool List< T >::isEmpty() const noexcept
  {
    return size_ == 0;
  }

  template< class T > typename List< T >::iter List< T >::begin()
  {
    if (size_) {
      return iter(head_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::iter List< T >::end()
  {
    if (size_) {
      return iter(head_->prev_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::citer List< T >::begin() const
  {
    if (size_) {
      return citer(head_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::citer List< T >::end() const
  {
    if (size_) {
      return citer(head_->prev_);
    }
    throw std::logic_error("empty list");
  }

  template< class T > typename List< T >::iter List< T >::addFront(const T &val)
  {
    node< T >* newNode = new node< T >(val);
    if (isEmpty()) {
      newNode->next_ = newNode;
      newNode->prev_ = newNode;
    } else {
      node< T >* tail_ = head_->prev_;
      newNode->next_ = head_;
      newNode->prev_ = tail_;
      tail_->next_ = newNode;
      head_->prev_ = newNode;
    }
    head_ = newNode;
    size_++;
    return iter(newNode);
  }

  template< class T > typename List< T >::iter List< T >::addEnd(const T &val)
  {
    node< T >* newNode = new node< T >(val);
    if (isEmpty()) {
      newNode->next_ = newNode;
      newNode->prev_ = newNode;
      head_ = newNode;
    } else {
      node< T >* tail_ = head_->prev_;
      newNode->next_ = head_;
      newNode->prev_ = tail_;
      tail_->next_ = newNode;
      head_->prev_ = newNode;
    }
    size_++;
    return iter(newNode);
  }

  template< class T > typename List< T >::iter List< T >::insertBefore(iter pos, const T &val)
  {
    node< T >* newNode = new node< T >(val);
    newNode->next_ = pos.node_;
    newNode->prev_ = pos.node_->prev_;
    pos.node_->prev_->next_ = newNode;
    pos.node_->prev_ = newNode;
    size_++;
    return iter(newNode);
  }

  template< class T > typename List< T >::iter List< T >::insertAfter(iter pos, const T &val)
  {
    node< T >* newNode = new node< T >(val);
    newNode->prev_ = pos.node_;
    newNode->next_ = pos.node_->next_;
    pos.node_->next_->prev_ = newNode;
    pos.node_->next_ = newNode;
    size_++;
    return iter(newNode);
  }

  template< class T > void List< T >::popFront() noexcept
  {
    if (!isEmpty()) {
      node< T >* newHead = head_->next_;
      head_->next_->prev_ = head_->prev_;
      head_->prev_->next_ = head_->next_;
      delete head_;
      if (size_ == 1) {
        head_ = nullptr;
      } else {
        head_ = newHead;
      }
      size_--;
    }
  }

  template< class T > void List< T >::popEnd() noexcept
  {
    if (!isEmpty()) {
      node< T >* toDel = head_->prev_;
      head_->prev_->prev_->next_ = head_;
      head_->prev_ = head_->prev_->prev_;
      delete toDel;
      if (size_ == 1) {
        head_ = nullptr;
      }
      size_--;
    }
  }

  template< class T > void List< T >::erase(iter pos) noexcept
  {
    pos.node_->next_->prev_ = pos.node_->prev_;
    pos.node_->prev_->next_ = pos.node_->next_;
    if (pos.node_ == head_) {
      head_ = head_->next_;
    }
    delete pos.node_;
    if (size_ == 1) {
      head_ = nullptr;
    }
    size_--;
  }

  template< class T > void List< T >::clear() noexcept
  {
    while (!isEmpty()) {
      popFront();
    }
  }

  template< class T > void List< T >::clear(iter from, iter to_included) noexcept
  {
    while(from != to_included) {
      iter next = iter(from.node_->next_);
      erase(from);
      from = next;
    }
    erase(from);
  }

  template< class T > List< T >& List< T >::operator=(List &other)
  {
    clear();
    if (other.isEmpty()) {
      return *this;
    }
    addFront(other.head_->val_);
    node< T >* toAdd = other.head_->next_;
    iter cur = iter(head_);
    while(toAdd != other.head_) {
      insertAfter(cur, toAdd->val_);
      toAdd = toAdd->next_;
      ++cur;
    }
    return *this;
  }

  template< class T > List< T >& List< T >::operator=(List &&other)
  {
    clear();
    head_ = other.head_;
    other.head_ = nullptr;
    size_ = other.size_;
    other.size_ = 0;
    return *this;
  }
}

#endif
