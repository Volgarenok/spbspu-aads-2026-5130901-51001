#ifndef LIST_IMPL_HPP
#define LIST_IMPL_HPP

#include <stdexcept>
#include "list.hpp"

namespace loseva {

template <typename T>
List<T>::Node::Node(const T &value):
  value(value),
  next(nullptr),
  prev(nullptr)
{
}

template <typename T>
List<T>::List():
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{
}

template <typename T>
List<T>::List (const List &other):
  head_(nullptr),
  tail_(nullptr),
  size_(0)
{
  Node *current = other.head_;

  while (current != nullptr) {
    pushBack(current->value);
    current = current->next;
  }
}

template <typename T>
List<T> &List<T>::operator=(const List &other)
{
  if (this != &other) {
    clear();

    Node *current = other.head_;

    while (current != nullptr) {
      pushBack(current->value);
      current = current->next;
    }
  }

  return *this;
}

template <typename T>
List<T>::~List()
{
  clear();
}

template <typename T>
void List<T>::pushBack(const T &value)
{
  Node *node = new Node(value);

  if (tail_ == nullptr) {
    head_ = node;
    tail_ = node;
  } else {
    node->prev = tail_;
    tail_->next = node;
    tail_ = node;
  }

  ++size_;
}

template <typename T>
void List<T>::pushFront(const T &value)
{
  Node *node = new Node(value);

  if (head_ == nullptr) {
    head_ = node;
    tail_ = node;
  } else {
    node->next = head_;
    head_->prev = node;
    head_ = node;
  }

  ++size_;
}

template <typename T>
void List<T>::popBack()
{
  if (tail_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  Node *node = tail_;
  tail_ = tail_->prev;

  if (tail_ != nullptr) {
    tail_->next = nullptr;
  } else {
    head_ = nullptr;
  }

  delete node;

  --size_;
}

template <typename T>
void List<T>::popFront()
{
  if (head_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  Node *node = head_;
  head_ = head_->next;

  if (head_ != nullptr) {
    head_->prev = nullptr;
  } else {
    tail_ = nullptr;
  }

  delete node;

  --size_;
}

template <typename T>
std::size_t List<T>::size() const
{
  return size_;
}

template <typename T>
bool List<T>::empty() const
{
  return size_ == 0;
}

template <typename T>
T &List<T>::front()
{
  if (head_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  return head_->value;
}

template <typename T>
const T &List<T>::front() const
{
  if (head_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  return head_->value;
}

template <typename T>
T &List<T>::back()
{
  if (tail_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  return tail_->value;
}

template <typename T>
const T &List<T>::back() const
{
  if (tail_ == nullptr) {
    throw std::out_of_range("List is empty");
  }

  return tail_->value;
}

template <typename T>
void List<T>::clear()
{
  Node *current = head_;

  while (current != nullptr) {
    Node *next = current->next;
    delete current;
    current = next;
  }

  head_ = nullptr;
  tail_ = nullptr;
  size_ = 0;
}

}

#endif
