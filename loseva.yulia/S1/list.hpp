#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>

namespace loseva {
template <typename T>
class List {
private:
  struct Node {
    T data;
    Node* next;
    Node* prev;

    Node(const T& value, Node* n = nullptr, Node* p = nullptr) : data(value), next(n), prev(p) {}
    Node(T&& value, Node* n = nullptr, Node* p = nullptr) : data(std::move(value)), next(n), prev(p) {}
  };

  Node* head;
  Node* tail;
  size_t size;
public:
  List();
  explicit List(size_t count, const T& value = T());
  List(std::initializer_list<T> init);
  ~List();
  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  bool empty() const noexcept;
  size_t get_size() const noexcept;
  void push_front(const T& value);
  void push_front(T&& value);
  void push_back(const T& value);
  void push_back(T&& value);
  void pop_front();
  void pop_back();
  void clear();
  };
}
