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
  void clear() noexcept;
  void copy_from(const List& other);
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
  iterator insert(const_iterator pos, const T& value);
  iterator insert(const_iterator pos, T&& value);
  iterator insert(const_iterator pos, size_t count, const T& value);
  template<typename InputIt>
  iterator insert(const_iterator pos, InputIt first, InputIt last);
  iterator insert(const_iterator pos, std::initializer_list<T> init);
  template<typename... Args>
  iterator emplace(const_iterator pos, Args&&... args);
  iterator erase(const_iterator pos);
  iterator erase(const_iterator first, const_iterator last);
  void splice(const_iterator pos, List& other);
  void splice(const_iterator pos, List&& other);
  void splice(const_iterator pos, List& other, const_iterator it);
  void splice(const_iterator pos, List&& other, const_iterator it);
  void splice(const_iterator pos, List& other, const_iterator first, const_iterator last);
  void splice(const_iterator pos, List&& other, const_iterator first, const_iterator last);

  void remove(const T& value);
  template<typename Predicate>
  void remove_if(Predicate pred);

  void reverse() noexcept;
  void unique();
  template<typename BinaryPredicate>
  void unique(BinaryPredicate pred);
  void sort();
  template<typename Compare>
  void sort(Compare comp);
  private:
    void merge_sort(Node*& start);
    Node* merge(Node* left, Node* right);
    template<typename Compare>
    Node* merge(Node* left, Node* right, Compare comp);
    void split(Node* source, Node*& front, Node*& back);

  template<typename... Args>
  void emplace_front(Args&&... args);
  template<typename... Args>
  void emplace_back(Args&&... args);
  void resize(size_t count);
  void resize(size_t count, const T& value);
  void swap(List& other) noexcept;

  List();
  explicit List(size_t count, const T& value = T());
  List(std::initializer_list<T> init);
  ~List();

  List(const List& other);
  List(List&& other) noexcept;

  List& operator=(const List& other);
  List& operator=(List&& other) noexcept;
  List& operator=(std::initializer_list<T> init);

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
