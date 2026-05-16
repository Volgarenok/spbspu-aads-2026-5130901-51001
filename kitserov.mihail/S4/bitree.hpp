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

    void update() noexcept
    {
      n = this;
      if (n) {
        n -> height_ = 1 + std::max(n -> left_ -> height, n -> right_ -> height);
        n -> size_ = 1 + (n -> left_ -> size + n -> right_ -> size);
      }
    }

    BSTree* rotateLeft(BSTree* child)
    {
      BSTree* parent = child -> parent_;
      BSTree* grand  = parent -> parent_;
      parent -> right_ = child -> left_;
      if (child -> left_) {
        child -> left_ -> parent_ = parent;
      }
      child -> left_ = parent;
      parent -> parent_ = child;
      child -> parent_ = grand;
      if (grand) {
        if (grand -> left_ == parent) {
          grand -> left_ = child;
        } else {
          grand -> right_ = child;
        }
      }
      parent -> update();
      child -> update();
      grand -> update();
      return child;
    }
    BSTree* rotateRight(BSTree* child)
    {
      BSTree* parent = child -> parent_;
      BSTree* grand  = parent -> parent_;
      parent -> left_ = child -> right_;
      if (child -> right_) {
        child -> right_ -> parent_ = parent;
      }
      child -> right_ = parent;
      parent -> parent_ = child;
      child -> parent_ = grand;
      if (grand) {
        if (grand -> left_ == parent) {
          grand -> left_ = child;
        } else {
          grand -> right_ = child;
        }
      }
      parent -> update();
      child -> update();
      grand -> update();
      return child;
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