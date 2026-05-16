#ifndef BITREE_HPP
#define BITREE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <algorithm>

namespace kitserov
{
  template < class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
    std::pair<const Key, Value> data_;
    BSTree* left_;
    BSTree* right_;
    BSTree* parent_;
    size_t height_;
    Compare cmp_;
    size_t size_;

    BSTree(const Key& k, const Value& v, BSTree* p, const Compare& comp)
        : data_(k, v), left_(nullptr), right_(nullptr), parent_(p), height_(1), cmp_(comp), size_(1) {}

    void update(BSTree* n) noexcept
    {
      if (n) {
        n -> height_ = 1 + std::max(n -> left_ -> height, n -> right_ -> height);
        n -> size_ = 1 + (n -> left_ -> size + n -> right_ -> size);
      }
    }

  public:
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    BSTree() : data_(), left_(nullptr), right_(nullptr), parent_(nullptr), height_(0), cmp_(Compare()), size_(0) {}

    size_t height() noexcept
    {
        return n ? n -> height_ : 0;
    }
    size_t size() noexcept {
        return n ? n->size_ : 0;
    }
  };
}

#endif