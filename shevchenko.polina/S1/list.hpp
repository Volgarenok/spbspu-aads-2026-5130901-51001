#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace shevchenko
{
template< class T >
struct Node
{
  T data;
  Node< T >* next;
  Node< T >* prev;
  
  explicit Node(const T& value) :
  data(value),
  next(nullptr),
  prev(nullptr)
  {
  }
};


