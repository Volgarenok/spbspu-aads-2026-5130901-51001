#ifndef LOSEVA_LIST_HPP
#define LOSEVA_LIST_HPP

#include <cstddef>

namespace loseva
{

template<class T>
class List;

template<class T>
class LIter
{
  friend class List<T>;

private:
  struct Node;
  Node* node_;

  explicit LIter(Node* n);

public:
  LIter();

  T& operator*() const;
  LIter& operator++();
  LIter& operator--();

  bool operator==(const LIter& other) const;
  bool operator!=(const LIter& other) const;
};

template<class T>
class LCIter
{
  friend class List<T>;

private:
  struct Node;
  const Node* node_;

  explicit LCIter(const Node* n);

public:
  LCIter();

  const T& operator*() const;
  LCIter& operator++();
  LCIter& operator--();

  bool operator==(const LCIter& other) const;
  bool operator!=(const LCIter& other) const;
};

template<class T>
class List
{
private:

  struct Node
  {
    T data;
    Node* next;
    Node* prev;

    Node(const T& d);
  };

  Node* head_;
  Node* tail_;

public:

  using iterator = LIter<T>;
  using const_iterator = LCIter<T>;

  List();
  ~List();

  List(const List& other);
  List(List&& other) noexcept;

  List& operator=(const List& other);
  List& operator=(List&& other) noexcept;

  bool empty() const;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  T& front();
  T& back();

  const T& front() const;
  const T& back() const;

  void push_front(const T& value);
  void push_back(const T& value);

  void pop_front();
  void pop_back();

  iterator insert_after(iterator pos, const T& value);

  void clear();
};

}

#include "list_impl.hpp"

#endif
