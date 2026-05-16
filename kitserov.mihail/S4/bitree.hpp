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

    BSTree* find_root(const Key& k)
    {
      BSTree* cur = this;
      while (cur) {
        if (cmp_(k, cur -> data_.first)) {
          cur = cur -> left_;
        } else if (cmp_(cur -> data_.first, k)) {
          cur = cur -> right_;
        } else {
          return cur;
        }
      }
      return nullptr;
    }
    const BSTree* find_root(const Key& k) const
    {
      return const_cast< BSTree* >(this) -> find_root(k);
    }
    void clear(BSTree* n)
    {
      if (!n) return;
      clear(n -> left_);
      clear(n -> right_);
      delete n;
    }

  public:
    friend class BSTIterator< Key, Value >;
    friend class BSTConstIterator< Key, Value >;
    BSTree() : data_(), left_(nullptr), right_(nullptr), parent_(nullptr), height_(0), cmp_(Compare()), size_(0) {}
    ~BSTree()
    {
      clear();
    }
    BSTree(const BSTree& other)
    {
      copy(other);
    }
    BSTree(BSTree&& other) noexcept
    {
      swap(other);
    }
    BSTree& operator=(const BSTree& other)
    {
      copy(other);
      return *this;
    }
    BSTree& operator=(BSTree&& other)
    {
      swap(other);
      return *this;
    }

    void swap(BSTree&& other) noexcept
    {
      std::swap(data_, other.data_);
      std::swap(left_, other.left_);
      std::swap(right_, other.right_);
      std::swap(parent_, other.parent_);
      std::swap(height_, other.height_);
      std::swap(size_, other.size_);
      std::swap(cmp_, other.cmp_);
      if (left_) {
        left_ -> parent_ = this;
      }
      if (right_) {
        right_ -> parent_ = this;
      }
    }
    void copy(const BSTree& other)
    {
      if (this == &other)
      {
        return;
      }
      BSTree* tmp = new BSTree();
      tmp -> data_ = other -> data_;
      tmp -> height_ = other -> height_;
      tmp -> size_ = other -> size_;
      tmp -> cmp_ = other -> cmp_;
      tmp -> parent_ = other -> parent_;
      tmp -> left_ -> copy(other.left_);
      tmp -> right_ -> copy(other.right_);
      swap(tmp);
    }
    size_t height() noexcept
    {
      return n ? n -> height_ : 0;
    }
    size_t size() noexcept {
      return n ? n->size_ : 0;
    }
    void push(const Key& k, const Value& v)
    {
      BSTree* cur = this;
      BSTree* parent = parent_;
      bool left_child;
      while (cur) {
        parent = cur;
        if (cmp_(k, cur -> data_.first)) {
          cur = cur -> left_;
          left_child = true;
        } else if (cmp_(cur -> data_.first, k)) {
          cur = cur -> right_;
          left_child = false;
        } else {
          cur->data_.second = v;
          return;
        }
      }
      BSTree* newNode = new BSTree(k, v, parent, cmp_);
      if (parent) {
        if (left_child) {
          parent -> left_ = newNode;
        } else {
          parent -> right_ = newNode;
        }
      }
      while (parent) {
        parent -> update();
        parent = parent -> parent_;
      }
    }
  };
}

#endif