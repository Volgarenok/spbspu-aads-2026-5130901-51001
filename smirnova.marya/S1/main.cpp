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
  class List;

}

