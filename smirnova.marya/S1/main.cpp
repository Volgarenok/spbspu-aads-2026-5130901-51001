#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova
{
  template < typename T >
  struct Node
  {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d):
      data(d),
      next(nullptr),
      prev(nullptr) {}
    Node(T&& d):
      data(std::move(d)),
      next(nullptr),
      prev(nullptr)
    {}
  };

  template < typename T >
  class LIter;

  template < typename T >
  class LCIter;

  template < typename T >
  class List
  {
  private:
    Node< T >* sentinel;
    size_t count;

  public:
    List():
      count(0)
    {
      sentinel = new Node< T >(T());
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
    }

    void clear() noexcept {
      Node< T >* current = sentinel->next;
      while (current != sentinel) {
        Node< T >* tmp = current->next;
        delete current;
        current = tmp;
      }
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
      count = 0;
    }

    ~List()
    {
      clear();
      delete sentinel;
    }

    List(const List& other):
      List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
        {
          push_back(it.value());
        }
    }

    List(List&& other) noexcept:
      sentinel(other.sentinel),
      count(other.count)
    {
      other.sentinel = new Node< T >(T());
      other.sentinel->next = other.sentinel;
      other.sentinel->prev = other.sentinel;
      other.count = 0;
    }

    List& operator=(const List& other)
    {
      if (this != &other) {
        List tmp(other);
        swap(tmp);
      }
      return *this;
    }

    List& operator=(List&& other) noexcept
    {
      if (this != &other) {
        clear();
        delete sentinel;
        sentinel = other.sentinel;
        count = other.count;
        other.sentinel = new Node< T >(T());
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.count = 0;
      }
      return *this;
    }

}
