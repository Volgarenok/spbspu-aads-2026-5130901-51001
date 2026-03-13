#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <string>

namespace losev {

template<class T>
struct Node {
  T data;
  Node* next;
  Node(const T& value, Node* n = nullptr);
};

template<class T>
class LCIter {
  friend class List<T>;
private:
  const Node<T>* ptr_;
  explicit LCIter(const Node<T>* ptr = nullptr);
public:
  const T& operator*() const;
  const T* operator->() const;
  LCIter& operator++();
  LCIter operator++(int);
  bool operator==(const LCIter& other) const;
  bool operator!=(const LCIter& other) const;
  const T* operator->() const;
};

template<class T>
class LIter {
  friend class List<T>;
private:
  Node<T>* ptr_;
  explicit LIter(Node<T>* ptr = nullptr);
public:
  bool operator==(const LIter& other) const;
  bool operator!=(const LIter& other) const;
  T& operator*();
  const T& operator*() const;
  LIter& operator++();
  LIter operator++(int);
  T* operator->();
  const T* operator->() const;
};

template<class T>
class List {
public:
  using iterator = LIter<T>;
  iterator begin() { return iterator(head_); }
  iterator end() { return iterator(nullptr); }

  using const_iterator = LCIter<T>;
  const_iterator begin() const { return const_iterator(head_); }
  const_iterator end() const { return const_iterator(nullptr); }

  List() : head_(nullptr) {}

  List(const List& other);

  ~List();

  bool empty() const { return head_ == nullptr; }

  void push_front(const T& value) {
    head_ = new Node<T>(value, head_);
  }

  void pop_front() {
    if (head_ == nullptr) return;
    Node<T>* temp = head_;
    head_ = head_->next;
    delete temp;
  }

  void clear();

private:
  Node<T>* head_;

  void copy_from(const List& other);
};



}

#endif
