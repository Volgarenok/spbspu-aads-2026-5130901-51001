#ifndef LOSEVA_LIST_IMPL_HPP
#define LOSEVA_LIST_IMPL_HPP

namespace loseva
{

template<class T>
List<T>::Node::Node(const T& d):
  data(d),
  next(nullptr),
  prev(nullptr)
{}

template<class T>
LIter<T>::LIter():
  node_(nullptr)
{}

template<class T>
LIter<T>::LIter(Node* n):
  node_(n)
{}

template<class T>
T& LIter<T>::operator*() const
{
  return node_->data;
}

template<class T>
LIter<T>& LIter<T>::operator++()
{
  node_ = node_->next;
  return *this;
}

template<class T>
LIter<T>& LIter<T>::operator--()
{
  node_ = node_->prev;
  return *this;
}

template<class T>
bool LIter<T>::operator==(const LIter& other) const
{
  return node_ == other.node_;
}

template<class T>
bool LIter<T>::operator!=(const LIter& other) const
{
  return node_ != other.node_;
}

/* const iterator */

template<class T>
LCIter<T>::LCIter():
  node_(nullptr)
{}

template<class T>
LCIter<T>::LCIter(const Node* n):
  node_(n)
{}

template<class T>
const T& LCIter<T>::operator*() const
{
  return node_->data;
}

template<class T>
LCIter<T>& LCIter<T>::operator++()
{
  node_ = node_->next;
  return *this;
}

template<class T>
LCIter<T>& LCIter<T>::operator--()
{
  node_ = node_->prev;
  return *this;
}

template<class T>
bool LCIter<T>::operator==(const LCIter& other) const
{
  return node_ == other.node_;
}

template<class T>
bool LCIter<T>::operator!=(const LCIter& other) const
{
  return node_ != other.node_;
}

/* list */

template<class T>
List<T>::List():
  head_(nullptr),
  tail_(nullptr)
{}

template<class T>
List<T>::~List()
{
  clear();
}

template<class T>
bool List<T>::empty() const
{
  return head_ == nullptr;
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
T& List<T>::front()
{
  return head_->data;
}

template<class T>
T& List<T>::back()
{
  return tail_->data;
}

template<class T>
void List<T>::push_front(const T& value)
{
  Node* n = new Node(value);

  n->next = head_;

  if (head_)
    head_->prev = n;

  head_ = n;

  if (!tail_)
    tail_ = n;
}

template<class T>
void List<T>::push_back(const T& value)
{
  Node* n = new Node(value);

  n->prev = tail_;

  if (tail_)
    tail_->next = n;

  tail_ = n;

  if (!head_)
    head_ = n;
}

template<class T>
void List<T>::pop_front()
{
  if (!head_)
    return;

  Node* tmp = head_;
  head_ = head_->next;

  if (head_)
    head_->prev = nullptr;
  else
    tail_ = nullptr;

  delete tmp;
}

template<class T>
void List<T>::pop_back()
{
  if (!tail_)
    return;

  Node* tmp = tail_;
  tail_ = tail_->prev;

  if (tail_)
    tail_->next = nullptr;
  else
    head_ = nullptr;

  delete tmp;
}

template<class T>
typename List<T>::iterator
List<T>::insert_after(iterator pos, const T& value)
{
  if (!pos.node_)
    return end();

  Node* cur = pos.node_;
  Node* n = new Node(value);

  n->next = cur->next;
  n->prev = cur;

  if (cur->next)
    cur->next->prev = n;
  else
    tail_ = n;

  cur->next = n;

  return iterator(n);
}

template<class T>
void List<T>::clear()
{
  while (head_)
    pop_front();
}

}

#endif
