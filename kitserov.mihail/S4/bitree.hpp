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
    using iterator = BSTIterator<Key, Value, Compare>;
    using const_iterator = BSTConstIterator<Key, Value, Compare>;
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
    Value& get(const Key& k)
    {
      BSTree* n = find_root(k);
      if (!n) {
        throw std::out_of_range("BSTree::get: key not found");
      }
      return n -> data_.second;
    }
    const Value& get(const Key& k) const {
        return const_cast< BSTree* >(this) -> get(k);
    }
    bool contains(const Key& k) const {
        return find_root(k) != nullptr;
    }
    Value drop(const Key& k)
    {
      BSTree* n = find_root(k);
      if (!n) {
        throw std::out_of_range("BSTree::get: key not found");
      }
      Value removed = n -> data_.second;
      BSTree* child = nullptr;
      BSTree* parent = n -> parent_;
      if (!n -> left_ && !n -> right_) {
        if (parent -> left_ == n) {
          parent -> left_ = nullptr;
        } else {
          parent -> right_ = nullptr;
        }
      } else if (!n -> left_ || !n -> right_) {
        child = n -> left_ ? n -> left_ : n -> right_;
        child -> parent_ = parent;
        if (parent -> left_ == n) {
          parent -> left_ = child;
        } else {
          parent -> right_ = child;
        }
      } else {
        BSTree* succ = n -> right_;
        while (succ -> left_) {
          succ = succ -> left_;
        }
        if (succ -> parent_ != n) {
          if (succ -> right_) {
            succ -> right_ -> parent_ = succ -> parent_;
            succ -> parent_ -> left_ = succ -> right_;
          } else {
            n -> right_ = succ -> right_;
            if (succ -> right_) {
              succ -> right_ -> parent_ = n;
            }
          }
          succ -> left_ = n -> left_;
          if (n -> left_) {
            n -> left_ -> parent_ = succ;
          }
          if (n -> right_ != succ) {
            succ -> right_ = n -> right_;
            if (n -> right_) {
              n -> right_ -> parent_ = succ;
            }
          }
          succ -> parent_ = n -> parent_;
          if (n -> parent_ -> left_ == n) {
            n -> parent_ -> left_ = succ;
          } else {
            n -> parent_ -> right_ = succ;
          }
          child = succ;
          parent = succ -> parent_ ? succ->parent_ : succ;
        }
      }
      BSTree* update_node = parent;
      while (update_node) {
        update_node -> update();
        update_node = update_node -> parent_;
      }
      delete n;
      return removed;
    }
  };
  template < class Key, class Value, class Compare >
  class BSTIterator
  {
    using Node = BSTree<Key, Value, Compare>;
    Node* node_;
    Node* sentinel_;
    explicit BSTIterator(Node* n, Node* s) : node_(n), sentinel_(s) {}
    friend class BSTConstIterator< Key, Value, Compare >;
    friend class BSTree< Key, Value, Compare >;
  public:
    using value_type = std::pair< const Key, Value >;
    BSTIterator() : node_(nullptr), sentinel_(nullptr) {}
    value_type& operator*() const
    {
      return node_ -> data_;
    }
    value_type* operator->() const
    {
      return &(node_ -> data_);
    }
    BSTIterator& operator++()
    {
      if (node_ -> right_) {
        node_ = node_ -> right_;
        while (node_ -> left_) node_ = node_ -> left_;
      } else {
        Node* p = node_ -> parent_;
        while (p != sentinel_ && node_ == p -> right_) {
          node_ = p;
          p = p -> parent_;
        }
        node_ = p;
      }
      return *this;
    }
    BSTIterator operator++(int)
    {
      BSTIterator tmp(*this);
      ++(*this);
      return tmp;
    }
    bool operator==(const BSTIterator& other) const
    {
      return node == other.node;
    }
    bool operator!=(const BSTIterator& other) const
    {
      return node != other.node;
    }
  };
  template < class Key, class Value, class Compare >
  class BSTConstIterator
  {
    using Node = BSTree<Key, Value, Compare>;
    const Node* node_;
    const Node* sentinel_;
    explicit BSTConstIterator(const Node* n, const Node* s) : node_(n), sentinel_(s) {}
    friend class BSTree< Key, Value, Compare >;
  public:
    using value_type = const std::pair< const Key, Value >;
    BSTConstIterator() : node_(nullptr), sentinel_(nullptr) {}
    BSTConstIterator(const BSTIterator< Key, Value, Compare >& it) : node(it.node), sentinel(it.sentinel) {}
    value_type& operator*() const
    {
      return node_ -> data_;
    }
    value_type* operator->() const
    {
      return &(node_ -> data_);
    }
    BSTConstIterator& operator++()
    {
      if (node_ -> right_) {
        node_ = node_ -> right_;
        while (node_ -> left_) node_ = node_ -> left_;
      } else {
        Node* p = node_ -> parent_;
        while (p != sentinel_ && node_ == p -> right_) {
          node_ = p;
          p = p -> parent_;
        }
        node_ = p;
      }
      return *this;
    }
    BSTConstIterator operator++(int)
    {
      BSTConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }
    bool operator==(const BSTConstIterator& other) const
    {
      return node == other.node;
    }
    bool operator!=(const BSTConstIterator& other) const
    {
      return node != other.node;
    }
  };
}

#endif