#include "list.h"

namespace losev {

// Node
template<class T>
Node<T>::Node(const T& value, Node* n) : data(value), next(n) {}

// LIter
template<class T>
LIter<T>::LIter(Node<T>* ptr) : ptr_(ptr) {}

template<class T>
bool LIter<T>::operator==(const LIter& other) const {
  return ptr_ == other.ptr_;
}

template<class T>
bool LIter<T>::operator!=(const LIter& other) const {
  return ptr_ != other.ptr_;
}

template<class T>
T& LIter<T>::operator*() {
  return ptr_->data;
}

template<class T>
const T& LIter<T>::operator*() const {
  return ptr_->data;
}

template<class T>
LIter<T>& LIter<T>::operator++() {
  ptr_ = ptr_->next;
  return *this;
}

template<class T>
LIter<T> LIter<T>::operator++(int) {
  LIter<T> temp = *this;
  ptr_ = ptr_->next;
  return temp;
}

template<class T>
T* LIter<T>::operator->() { 
  return &(ptr_->data); 
}

template<class T>
const T* LIter<T>::operator->() const { 
  return &(ptr_->data); 
}

// LCIter
template<class T>
LCIter<T>::LCIter(const Node<T>* ptr) : ptr_(ptr) {}

template<class T>
const T& LCIter<T>::operator*() const {
  return ptr_->data;
}

template<class T>
const T* LCIter<T>::operator->() const {
  return &(ptr_->data);
}

template<class T>
LCIter<T>& LCIter<T>::operator++() {
  ptr_ = ptr_->next;
  return *this;
}

template<class T>
LCIter<T> LCIter<T>::operator++(int) {
  LCIter<T> temp = *this;
  ptr_ = ptr_->next;
  return temp;
}

template<class T>
bool LCIter<T>::operator==(const LCIter& other) const {
  return ptr_ == other.ptr_;
}

template<class T>
bool LCIter<T>::operator!=(const LCIter& other) const {
  return ptr_ != other.ptr_;
}

// List
template<class T>
List<T>::List(const List& other) : head_(nullptr) {
  copy_from(other);
}

template<class T>
List<T>::~List() {
  clear();
}

template<class T>
List<T>& List<T>::operator=(const List& other) {
  if (this != &other) {
    clear();
    copy_from(other);
  }
  return *this;
}

template<class T>
void List<T>::copy_from(const List& other) {
  head_ = nullptr;
  if (other.head_ == nullptr) {
    return;
  }
  head_ = new Node<T>(other.head_->data, nullptr);
  Node<T>* current_new = head_;
  Node<T>* current_other = other.head_->next;
  
  while (current_other != nullptr) {
    current_new->next = new Node<T>(current_other->data, nullptr);
    current_new = current_new->next;
    current_other = current_other->next;
  }
}

template<class T>
void List<T>::clear() {
  while (head_ != nullptr) {
    Node<T>* temp = head_;
    head_ = head_->next;
    delete temp;
  }
}

template struct Node<int>;
template struct Node<NamedList>;
template class LIter<int>;
template class LIter<NamedList>;
template class LCIter<int>;
template class LCIter<NamedList>;
template class List<int>;
template class List<NamedList>;
}
