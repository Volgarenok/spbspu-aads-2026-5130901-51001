#ifndef LOSEVA_LIST_IMPL_HPP
#define LOSEVA_LIST_IMPL_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace loseva {

  template <typename T>
  class List {
  private:
    struct Node {
      T data;
      Node* next;
      Node* prev;

      Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;

  public:
    class iterator {
      Node* ptr_;
    public:
      iterator(Node* p = nullptr) : ptr_(p) {}

      T& operator*() { return ptr_->data; }
      T* operator->() { return &ptr_->data; }

      iterator& operator++() { ptr_ = ptr_->next; return *this; }
      iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

      iterator& operator--() { ptr_ = ptr_->prev; return *this; }

      bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
      bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

      friend class List;
    };

    class const_iterator {
      const Node* ptr_;
    public:
      const_iterator(const Node* p = nullptr) : ptr_(p) {}

      const T& operator*() const { return ptr_->data; }

      const_iterator& operator++() { ptr_ = ptr_->next; return *this; }
      bool operator!=(const const_iterator& other) const {
        return ptr_ != other.ptr_;
      }

      friend class List;
    };

    List() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~List() {
      clear();
    }

    List(const List& other) : head_(nullptr), tail_(nullptr), size_(0) {
      for (auto it = other.begin(); it != other.end(); ++it) {
        push_back(*it);
      }
    }

    List& operator=(const List& other) {
      if (this != &other) {
        clear();
        for (auto it = other.begin(); it != other.end(); ++it) {
          push_back(*it);
        }
      }
      return *this;
    }

    List(List&& other) noexcept : head_(other.head_), tail_(other.tail_), size_(other.size_) {
      other.head_ = other.tail_ = nullptr;
      other.size_ = 0;
    }

    List& operator=(List&& other) noexcept {
      if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
      }
      return *this;
    }

    bool empty() const {
      return size_ == 0;
    }

    size_t size() const {
      return size_;
    }

    void clear() {
      while (!empty()) {
        pop_front();
      }
    }

    T& front() {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      return head_->data;
    }

    const T& front() const {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      return head_->data;
    }

    T& back() {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      return tail_->data;
    }

    const T& back() const {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      return tail_->data;
    }

    void push_back(const T& val) {
      Node* n = new Node(val);
      if (tail_) {
        tail_->next = n;
        n->prev = tail_;
        tail_ = n;
      } else {
        head_ = tail_ = n;
      }
      ++size_;
    }

    void push_front(const T& val) {
      Node* n = new Node(val);
      if (head_) {
        head_->prev = n;
        n->next = head_;
        head_ = n;
      } else {
        head_ = tail_ = n;
      }
      ++size_;
    }

    void pop_back() {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      Node* tmp = tail_;
      tail_ = tail_->prev;

      if (tail_) {
        tail_->next = nullptr;
      } else {
        head_ = nullptr;
      }
      delete tmp;
      --size_;
    }

    void pop_front() {
      if (empty()) {
        throw std::underflow_error("List is empty");
      }
      Node* tmp = head_;
      head_ = head_->next;

      if (head_) {
        head_->prev = nullptr;
      } else {
        tail_ = nullptr;
      }
      delete tmp;
      --size_;
    }

    iterator begin() { return iterator(head_); }
    iterator end() { return iterator(nullptr); }

    const_iterator begin() const { return const_iterator(head_); }
    const_iterator end() const { return const_iterator(nullptr); }

    iterator insert(iterator pos, const T& val) {
      if (pos.ptr_ == nullptr) {
        push_back(val);
        return iterator(tail_);
      }

      Node* cur = pos.ptr_;
      Node* n = new Node(val);

      n->next = cur;
      n->prev = cur->prev;

      if (cur->prev) {
        cur->prev->next = n;
      } else {
        head_ = n;
      }

      cur->prev = n;

      ++size_;
      return iterator(n);
    }

    iterator erase(iterator pos) {
      if (pos.ptr_ == nullptr) {
        return end();
      }
      Node* cur = pos.ptr_;
      Node* next = cur->next;

      if (cur->prev) {
        cur->prev->next = cur->next;
      } else {
        head_ = cur->next;
      }
      if (cur->next) {
        cur->next->prev = cur->prev;
      } else {
        tail_ = cur->prev;
      }
      delete cur;
      --size_;

      return iterator(next);
    }
  };

}

#endif
