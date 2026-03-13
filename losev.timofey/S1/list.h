#ifndef LIST_H
#define LIST_H

#include <cstddef>
#include <string>

namespace losev {

// Структура узла списка
template<class T>
struct Node {
  T data;
  Node* next;
  Node(const T& value, Node* n = nullptr);
};

template<class T>
class List {
public:
  List() : head_(nullptr) {}
  
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

private:
  Node<T>* head_;
};



}

#endif
