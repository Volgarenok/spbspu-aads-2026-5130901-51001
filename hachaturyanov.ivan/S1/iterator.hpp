#ifndef ITERATOR_HPP
#define ITERATOR_HPP

#include "node.hpp"

namespace hachaturyanov
{
  template< class T > class List;
  template< class T > class LIter {
    friend class List;
    explicit LIter(node* n);
    node* node_;
  public:
    T& operator*();
    LIter& operator++();
    LIter& operator--();
    bool operator==(const LIter& other) const;
    bool operator!=(const LIter& other) const;
  };
}

#endif
