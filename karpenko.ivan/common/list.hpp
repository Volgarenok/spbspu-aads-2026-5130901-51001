#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace karpenko {

template<typename T> class List;

namespace detail {

struct NodeBase {
  NodeBase* next_;
  explicit NodeBase(NodeBase* nxt = nullptr) noexcept : next_(nxt) {}
};

template<typename T>
struct Node final : NodeBase {
  T data_;
  explicit Node(NodeBase* nxt, const T& value) : NodeBase(nxt), data_(value) {}
  explicit Node(NodeBase* nxt, T&& value) : NodeBase(nxt), data_(std::move(value)) {}
};

}

template<typename T>
class LIter {
public:
  LIter() noexcept : ptr_(nullptr) {}
  explicit LIter(detail::NodeBase* p) noexcept : ptr_(p) {}

  T& operator*() const noexcept {
    auto* node_ptr = static_cast<detail::Node<T>*>(ptr_);
    return node_ptr->data_;
  }

  T* operator->() const noexcept {
    auto* node_ptr = static_cast<detail::Node<T>*>(ptr_);
    return &(node_ptr->data_);
  }

  LIter& operator++() noexcept {
    ptr_ = ptr_->next_;
    return *this;
  }

  LIter operator++(int) noexcept {
    LIter tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  bool operator==(const LIter& other) const noexcept { return ptr_ == other.ptr_; }
  bool operator!=(const LIter& other) const noexcept { return !(*this == other); }

  detail::NodeBase* get_ptr() const noexcept { return ptr_; }

private:
  detail::NodeBase* ptr_;
  friend class List<T>;
};

template<typename T>
class LCIter {
public:
  LCIter() noexcept : ptr_(nullptr) {}
  explicit LCIter(const detail::NodeBase* p) noexcept : ptr_(p) {}
  LCIter(const LIter<T>& it) noexcept : ptr_(it.get_ptr()) {}

  const T& operator*() const noexcept {
    auto* node_ptr = static_cast<const detail::Node<T>*>(ptr_);
    return node_ptr->data_;
  }

  const T* operator->() const noexcept {
    auto* node_ptr = static_cast<const detail::Node<T>*>(ptr_);
    return &(node_ptr->data_);
  }

  LCIter& operator++() noexcept {
    ptr_ = ptr_->next_;
    return *this;
  }

  LCIter operator++(int) noexcept {
    LCIter tmp = *this;
    ptr_ = ptr_->next_;
    return tmp;
  }

  bool operator==(const LCIter& other) const noexcept { return ptr_ == other.ptr_; }
  bool operator!=(const LCIter& other) const noexcept { return !(*this == other); }

private:
  const detail::NodeBase* ptr_;
  friend class List<T>;
};

template<typename T>
class List {
public:
  using iterator = LIter<T>;
  using const_iterator = LCIter<T>;
  using size_type = std::size_t;

  List() : head_(new detail::NodeBase), tail_(head_) {
    head_->next_ = head_;
  }

  ~List() {
    clear();
    delete head_;
  }

  List(const List& other) : List() {
    for (const_iterator it = other.cbegin(); it != other.cend(); ++it) {
      push_back(*it);
    }
  }

  List(List&& other) noexcept : head_(other.head_), tail_(other.tail_) {
    other.head_ = new detail::NodeBase;
    other.head_->next_ = other.head_;
    other.tail_ = other.head_;
  }

  List& operator=(const List& other) {
    if (this != &other) {
      List tmp(other);
      swap(tmp);
    }
    return *this;
  }

  List& operator=(List&& other) noexcept {
    if (this != &other) {
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

  iterator begin() noexcept { return iterator(head_->next_); }
  const_iterator begin() const noexcept { return const_iterator(head_->next_); }
  iterator end() noexcept { return iterator(head_); }
  const_iterator end() const noexcept { return const_iterator(head_); }
  const_iterator cbegin() const noexcept { return const_iterator(head_->next_); }
  const_iterator cend() const noexcept { return const_iterator(head_); }

  bool empty() const noexcept { return head_->next_ == head_; }

  T& front() {
    if (empty()) throw std::out_of_range("List is empty");
    return *begin();
  }
  const T& front() const {
    if (empty()) throw std::out_of_range("List is empty");
    return *begin();
  }
  T& back() {
    if (empty()) throw std::out_of_range("List is empty");
    return *iterator(tail_);
  }
  const T& back() const {
    if (empty()) throw std::out_of_range("List is empty");
    return *const_iterator(tail_);
  }

  void push_front(const T& value) { insert_after(iterator(head_), value); }
  void push_front(T&& value) { insert_after(iterator(head_), std::move(value)); }

  void push_back(const T& value) {
    if (empty()) push_front(value);
    else insert_after(iterator(tail_), value);
  }
  void push_back(T&& value) {
    if (empty()) push_front(std::move(value));
    else insert_after(iterator(tail_), std::move(value));
  }

  void pop_front() {
    if (empty()) throw std::out_of_range("List is empty");
    detail::NodeBase* old = head_->next_;
    head_->next_ = old->next_;
    if (tail_ == old) tail_ = head_;
    auto* node_to_delete = static_cast<detail::Node<T>*>(old);
    delete node_to_delete;
  }

  void pop_back() {
    if (empty()) throw std::out_of_range("List is empty");
    if (head_->next_ == tail_) {
      pop_front();
      return;
    }
    detail::NodeBase* prev = head_->next_;
    while (prev->next_ != tail_) prev = prev->next_;
    auto* node_to_delete = static_cast<detail::Node<T>*>(tail_);
    delete node_to_delete;
    tail_ = prev;
    tail_->next_ = head_;
  }

  iterator insert_after(iterator pos, const T& value) {
    auto* new_node = new detail::Node<T>(pos.get_ptr()->next_, value);
    pos.get_ptr()->next_ = new_node;
    if (tail_ == pos.get_ptr()) tail_ = new_node;
    return iterator(new_node);
  }

  iterator insert_after(iterator pos, T&& value) {
    auto* new_node = new detail::Node<T>(pos.get_ptr()->next_, std::move(value));
    pos.get_ptr()->next_ = new_node;
    if (tail_ == pos.get_ptr()) tail_ = new_node;
    return iterator(new_node);
  }

  iterator erase_after(iterator pos) {
    if (pos.get_ptr() == head_ || pos.get_ptr()->next_ == head_)
      throw std::out_of_range("Invalid position for erase_after");
    detail::NodeBase* to_delete = pos.get_ptr()->next_;
    pos.get_ptr()->next_ = to_delete->next_;
    if (tail_ == to_delete) tail_ = pos.get_ptr();
    auto* node_to_delete = static_cast<detail::Node<T>*>(to_delete);
    delete node_to_delete;
    return iterator(pos.get_ptr()->next_);
  }

  void clear() noexcept {
    while (!empty()) pop_front();
  }

  void swap(List& other) noexcept {
    std::swap(head_, other.head_);
    std::swap(tail_, other.tail_);
  }

  size_type size() const noexcept {
    size_type count = 0;
    for (const_iterator it = cbegin(); it != cend(); ++it) ++count;
    return count;
  }

private:
  detail::NodeBase* head_;
  detail::NodeBase* tail_;
};

template<typename T>
void swap(List<T>& lhs, List<T>& rhs) noexcept { lhs.swap(rhs); }

}

#endif

