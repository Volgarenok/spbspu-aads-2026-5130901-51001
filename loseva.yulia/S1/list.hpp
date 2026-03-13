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
  class Iterator {
    friend class List<T>;
    Node* current;
    explicit Iterator(Node* node);
  public:
     Iterator();
     Iterator& operator++();
     Iterator& operator--();
     Iterator operator++(int);
     Iterator operator--(int);
     T& operator*() const;
     T* operator->() const;
     bool operator==(const Iterator& other) const;
     bool operator!=(const Iterator& other) const;
  };

  class ConstIterator {
    friend class List<T>;
    const Node* current;
    explicit ConstIterator(const Node* node);

  public:
    ConstIterator();
    ConstIterator(const Iterator& it);

    ConstIterator& operator++();
    ConstIterator& operator--();

    ConstIterator operator++(int);
    ConstIterator operator--(int);

    const T& operator*() const;
    const T* operator->() const;

    bool operator==(const ConstIterator& other) const;
    bool operator!=(const ConstIterator& other) const;
  };

  using iterator = Iterator;
  using const_iterator = ConstIterator;
  using reverse_iterator = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;
  iterator begin() noexcept;
  const_iterator begin() const noexcept;
  const_iterator cbegin() const noexcept;

  iterator end() noexcept;
  const_iterator end() const noexcept;
  const_iterator cend() const noexcept;
  reverse_iterator rbegin() noexcept;
  const_reverse_iterator rbegin() const noexcept;
  const_reverse_iterator crbegin() const noexcept;
  reverse_iterator rend() noexcept;
  const_reverse_iterator rend() const noexcept;
  const_reverse_iterator crend() const noexcept;

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
#include "list_impl.hpp"

#endif // LIST_HPP
