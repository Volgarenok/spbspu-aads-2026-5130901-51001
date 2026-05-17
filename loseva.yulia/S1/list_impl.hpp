#ifndef LOSEVA_LIST_IMPL_HPP
#define LOSEVA_LIST_IMPL_HPP
#include <iostream>
#include "list.hpp"

namespace loseva
{

template<class T>
Node<T>::Node(const T& value, Node* n):
  data(value),
  next(n)
{}

template<class T>
List<T>::List():
  head_(nullptr)
{}

template<class T>
List<T>::List(const List& other):
  head_(nullptr)
{
  copy_from(other);
}

template<class T>
List<T>::~List()
{
  clear();
}

template<class T>
typename List<T>::iterator List<T>::begin()
{
  return iterator(head_);
}

template<class T>
typename List<T>::iterator List<T>::end()
{
  return iterator(nullptr);
}

template<class T>
typename List<T>::const_iterator List<T>::begin() const
{
  return const_iterator(head_);
}

template<class T>
typename List<T>::const_iterator List<T>::end() const
{
  return const_iterator(nullptr);
}

template<class T>
bool List<T>::empty() const
{
  return head_ == nullptr;
}

template<class T>
void List<T>::push_front(const T& value)
{
  head_ = new Node<T>(value, head_);
}

template<class T>
void List<T>::pop_front()
{
  if (!head_)
  {
    return;
  }

  Node<T>* temp = head_;
  head_ = head_->next;
  delete temp;
}

template<class T>
List<T>& List<T>::operator=(const List& other)
{
  if (this != &other)
  {
    clear();
    copy_from(other);
  }
  return *this;
}

template<class T>
void List<T>::copy_from(const List& other)
{
  if (!other.head_)
  {
    return;
  }

  head_ = new Node<T>(other.head_->data);

  Node<T>* cur_new = head_;
  Node<T>* cur_other = other.head_->next;

  while (cur_other)
  {
    cur_new->next = new Node<T>(cur_other->data);
    cur_new = cur_new->next;
    cur_other = cur_other->next;
  }
}

template<class T>
void List<T>::clear()
{
  while (head_)
  {
    Node<T>* temp = head_;
    head_ = head_->next;
    delete temp;
  }
}


template<class T>
LIter<T>::LIter(Node<T>* ptr):
  ptr_(ptr)
{}

template<class T>
bool LIter<T>::operator==(const LIter& other) const
{
  return ptr_ == other.ptr_;
}

template<class T>
bool LIter<T>::operator!=(const LIter& other) const
{
  return ptr_ != other.ptr_;
}

template<class T>
T& LIter<T>::operator*()
{
  return ptr_->data;
}

template<class T>
const T& LIter<T>::operator*() const
{
  return ptr_->data;
}

template<class T>
LIter<T>& LIter<T>::operator++()
{
  ptr_ = ptr_->next;
  return *this;
}

template<class T>
LIter<T> LIter<T>::operator++(int)
{
  LIter temp = *this;
  ptr_ = ptr_->next;
  return temp;
}

template<class T>
T* LIter<T>::operator->()
{
  return &(ptr_->data);
}

template<class T>
const T* LIter<T>::operator->() const
{
  return &(ptr_->data);
}


template<class T>
LCIter<T>::LCIter(const Node<T>* ptr):
  ptr_(ptr)
{}

template<class T>
LCIter<T>::LCIter(const LIter<T>& iter):
  ptr_(iter.ptr_)
{}

template<class T>
const T& LCIter<T>::operator*() const
{
  return ptr_->data;
}

template<class T>
const T* LCIter<T>::operator->() const
{
  return &(ptr_->data);
}

template<class T>
LCIter<T>& LCIter<T>::operator++()
{
  ptr_ = ptr_->next;
  return *this;
}

template<class T>
LCIter<T> LCIter<T>::operator++(int)
{
  LCIter temp = *this;
  ptr_ = ptr_->next;
  return temp;
}

template<class T>
bool LCIter<T>::operator==(const LCIter& other) const
{
  return ptr_ == other.ptr_;
}

template<class T>
bool LCIter<T>::operator!=(const LCIter& other) const
{
  return ptr_ != other.ptr_;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const LIter<T>&)
{
  os << "LIter";
  return os;
}

template<class T>
std::ostream& operator<<(std::ostream& os, const LCIter<T>&)
{
  os << "LCIter";
  return os;
}

}

#endif
