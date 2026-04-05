#ifndef LIST_HPP
#define LIST_HPP

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
<<<<<<< HEAD
    Node(const T& d):
      data(d),
      next(nullptr),
      prev(nullptr) {}
    Node(T&& d):
      data(std::move(d)),
      next(nullptr),
      prev(nullptr)
    {}
=======

    Node() : next(nullptr), prev(nullptr) {}  // sentinel

    Node(const T& d)
      : data(d), next(nullptr), prev(nullptr) {}

    Node(T&& d)
      : data(std::move(d)), next(nullptr), prev(nullptr) {}
>>>>>>> smirnova.marya/S1
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
<<<<<<< HEAD
      sentinel = new Node< T >(T());
=======
      sentinel = new Node< T >();
>>>>>>> smirnova.marya/S1
      sentinel->next = sentinel;
      sentinel->prev = sentinel;
    }

    void clear() noexcept
    {
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
<<<<<<< HEAD
          push_back(it.value());
=======
          pushBack(it.value());
>>>>>>> smirnova.marya/S1
        }
    }

    List(List&& other) noexcept:
      sentinel(other.sentinel),
      count(other.count)
    {
<<<<<<< HEAD
      other.sentinel = new Node< T >(T());
=======
      other.sentinel = new Node<T>(T());
>>>>>>> smirnova.marya/S1
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
<<<<<<< HEAD
        sentinel = other.sentinel;
        count = other.count;
        other.sentinel = new Node< T >(T());
=======

        sentinel = other.sentinel;
        count = other.count;

        other.sentinel = new Node<T>(T());
>>>>>>> smirnova.marya/S1
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.count = 0;
      }
      return *this;
    }

    void swap(List& other) noexcept
    {
      std::swap(sentinel, other.sentinel);
      std::swap(count, other.count);
    }

<<<<<<< HEAD
=======
    Node<T>* frontNode()
    {
      return sentinel->next;
    }

    Node<T>* backNode()
    {
      return sentinel->prev;
    }

>>>>>>> smirnova.marya/S1
    bool empty() const noexcept
    {
      return count == 0;
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    size_t size() const noexcept
    {
      return count;
    }

<<<<<<< HEAD
=======
    T& front()
    {
      if (empty()) throw std::out_of_range("empty");
      return sentinel->next->data;
    }

    const T& front() const
    {
      if (empty()) throw std::out_of_range("empty");
      return sentinel->next->data;
    }

    T& back()
    {
      if (empty()) throw std::out_of_range("empty");
      return sentinel->prev->data;
    }

    const T& back() const
    {
      if (empty()) throw std::out_of_range("empty");
      return sentinel->prev->data;
    }

>>>>>>> smirnova.marya/S1
    void pushBack(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void pushBack(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->next = sentinel;
      n->prev = sentinel->prev;
      sentinel->prev->next = n;
      sentinel->prev = n;
      ++count;
    }

    void pushFront(const T& val)
    {
      Node< T >* n = new Node< T >(val);
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void pushFront(T&& val)
    {
      Node< T >* n = new Node< T >(std::move(val));
      n->prev = sentinel;
      n->next = sentinel->next;
      sentinel->next->prev = n;
      sentinel->next = n;
      ++count;
    }

    void popFront()
    {
      if (empty()) {
        throw std::out_of_range("pop_front on empty list");
      }
      Node< T >* tmp = sentinel->next;
      sentinel->next = tmp->next;
      tmp->next->prev = sentinel;
      delete tmp;
      --count;
    }

    void popBack()
    {
      if (empty()) {
        throw std::out_of_range("pop_back on empty list");
      }
      Node< T >* tmp = sentinel->prev;
      sentinel->prev = tmp->prev;
      tmp->prev->next = sentinel;
      delete tmp;
      --count;
    }

    LIter<T> begin()
    {
      return LIter<T>(sentinel->next, sentinel);
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    LIter<T> end()
    {
      return LIter<T>(sentinel, sentinel);
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    LCIter<T> begin() const
    {
      return LCIter<T>(sentinel->next, sentinel);
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    LCIter<T> end() const
    {
      return LCIter<T>(sentinel, sentinel);
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    LCIter<T> cbegin() const
    {
      return LCIter<T>(sentinel->next, sentinel);
    }
<<<<<<< HEAD
=======

>>>>>>> smirnova.marya/S1
    LCIter<T> cend() const
    {
      return LCIter<T>(sentinel, sentinel);
    }
  };

  template < typename T >
  class LIter
  {
  private:
<<<<<<< HEAD
      Node< T >* node;
      Node< T >* sentinel;
  public:
    LIter(Node< T >* n = nullptr, Node< T >* s = nullptr):
      node(n),
      sentinel(s)
    {}
=======
    Node< T >* node;
    Node< T >* sentinel;

  public:
    LIter(Node< T >* n = nullptr, Node< T >* s = nullptr)
      : node(n), sentinel(s)
    {}

>>>>>>> smirnova.marya/S1
    bool valid() const
    {
      return node != sentinel;
    }
<<<<<<< HEAD
    void next()
    {
      if(node) node = node->next;
    }
    T& value()
    {
      return node->data;
    }
    const T& value() const
    {
=======

    void next()
    {
      if (!valid()) {
        throw std::out_of_range("iterator cannot advance past end()");
      }
      node = node->next;
    }

    T& value()
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
      return node->data;
    }

    const T& value() const
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
>>>>>>> smirnova.marya/S1
      return node->data;
    }
  };

  template < typename T >
  class LCIter
  {
  private:
    const Node< T >* node;
    const Node< T >* sentinel;
<<<<<<< HEAD
  public:
    LCIter(const Node< T >* n = nullptr, const Node< T >* s = nullptr):
      node(n),
      sentinel(s)
    {}
=======

  public:
    LCIter(const Node< T >* n = nullptr, const Node< T >* s = nullptr)
      : node(n), sentinel(s)
    {}

>>>>>>> smirnova.marya/S1
    bool valid() const
    {
      return node != sentinel;
    }
<<<<<<< HEAD
    void next()
    {
      if(node) node = node->next;
    }
    const T& value() const
    {
=======

    void next()
    {
      if (node) {
        node = node->next;
      }
    }

    const T& value() const
    {
      if (!valid()) {
        throw std::out_of_range("iterator is end()");
      }
>>>>>>> smirnova.marya/S1
      return node->data;
    }
  };
}

#endif

