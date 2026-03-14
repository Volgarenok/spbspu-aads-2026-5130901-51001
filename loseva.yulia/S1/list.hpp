#ifndef LOSEVA_LIST_HPP
#define LOSEVA_LIST_HPP

#include <cstddef>
#include <string>
#include <ostream>
#include "sequence.hpp"

namespace loseva
{

template<class T> class List;
template<class T> class LIter;
template<class T> class LCIter;

template<class T>
struct Node
{
  T data;
  Node* next;

  Node(const T& value, Node* n = nullptr);
};

template<class T>
class LIter
{
  friend class List<T>;
  friend class LCIter<T>;

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
class LCIter
{
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

  LCIter(const LIter<T>& iter);
};

template<class T>
class List : public Sequence<T>
{
public:
  using iterator = LIter<T>;
  using const_iterator = LCIter<T>;

  iterator begin();
  iterator end();

  const_iterator begin() const;
  const_iterator end() const;

  List();
  List(const List& other);
  ~List();

  bool empty() const override;

  void push_front(const T& value) override;
  void pop_front() override;

  List& operator=(const List& other);

  void clear();

private:
  Node<T>* head_;
  void copy_from(const List& other);
};

struct NamedList
{
  std::string name;
  List<size_t> numbers;
};

template<class T>
std::ostream& operator<<(std::ostream& os, const LIter<T>&);

template<class T>
std::ostream& operator<<(std::ostream& os, const LCIter<T>&);

}

#include "list_impl.hpp"

#endif
