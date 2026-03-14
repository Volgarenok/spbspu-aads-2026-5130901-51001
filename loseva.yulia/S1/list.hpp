#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <iterator>

#include "sequence.hpp"

namespace loseva {

template <typename T>
class List: public Sequence<T>
{
public:
  List();
  List(const List &other);
  List &operator=(const List &other);
  ~List();

  void pushBack(const T &value) override;
  void pushFront(const T &value) override;

  void popBack() override;
  void popFront() override;

  std::size_t size() const override;
  bool empty() const override;

  T &front() override;
  const T &front() const override;

  T &back() override;
  const T &back() const override;

  void clear();

private:
  struct Node
  {
    T value;
    Node *next;
    Node *prev;

    explicit Node(const T &value);
  };

  Node *head_;
  Node *tail_;
  std::size_t size_;
};

}

#include "list_impl.hpp"

#endif
