#ifndef MUSORIN_LIST_HPP
#define MUSORIN_LIST_HPP
#include <cstddef>
#include "iterator.hpp"
#include "node.hpp"
namespace musorin {
template< class T >
class List {
public:
  using iterator = LIter< T >;
  using const_iterator = LCIter< T >;
  List():
    head_(nullptr),
    tail_(nullptr),
    size_(0)
  {}
  ~List()
  {
    clear();
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
  T & front()
  {
    return head_->value_;
  }
  const T & front() const
  {
    return head_->value_;
  }
  T & back()
  {
    return tail_->value_;
  }
  const T & back() const
  {
    return tail_->value_;
  }
  bool empty() const
  {
    return size_ == 0;
  }
  std::size_t size() const
  {
    return size_;
  }
  void pushFront(const T & value)
  {
    Node * node = new Node(value);
    linkFront(node);
  }
  void pushBack(const T & value)
  {
    Node * node = new Node(value);
    linkBack(node);
  }
  void clear()
  {
    while (head_ != nullptr) {
      Node * tmp = head_;
      head_ = head_->next_;
      delete tmp;
    }
    tail_ = nullptr;
    size_ = 0;
  }
private:
  using Node = detail::Node< T >;
  void linkFront(Node * node)
  {
    if (head_ == nullptr) {
      head_ = node;
      tail_ = node;
    } else {
      node->next_ = head_;
      head_->prev_ = node;
      head_ = node;
    }
    ++size_;
  }
  void linkBack(Node * node)
  {
    if (tail_ == nullptr) {
      head_ = node;
      tail_ = node;
    } else {
      node->prev_ = tail_;
      tail_->next_ = node;
      tail_ = node;
    }
    ++size_;
  }
  Node * head_;
  Node * tail_;
  std::size_t size_;
};
}
#endif
