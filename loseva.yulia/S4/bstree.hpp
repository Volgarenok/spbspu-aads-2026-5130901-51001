#ifndef LOSEVA_BSTREE_HPP
#define LOSEVA_BSTREE_HPP

#include <utility>
#include <stdexcept>
#include <functional>
#include <algorithm>

namespace loseva {

template<class Key, class Value>
struct Node {
  std::pair<const Key, Value> data_;
  Node* parent_;
  Node* left_;
  Node* right_;

  Node(const Key& k, const Value& v, Node* parent = nullptr)
    : data_(k, v), parent_(parent), left_(nullptr), right_(nullptr) {}
};

template<class Key, class Value>
class BSTConstIterator {
public:
  using value_type = const std::pair<const Key, Value>;
  using pointer = value_type*;
  using reference = value_type&;

  BSTConstIterator(Node<Key, Value>* node = nullptr)
    : current_(node) {}

  reference operator*() const {
    return current_->data_;
  }

  pointer operator->() const {
    return &(current_->data_);
  }

  BSTConstIterator& operator++() {
    if (current_->right_) {
      current_ = current_->right_;
      while (current_->left_) {
        current_ = current_->left_;
      }
    } else {
      Node<Key, Value>* p = current_->parent_;
      while (p && current_ == p->right_) {
        current_ = p;
        p = p->parent_;
      }
      current_ = p;
    }
    return *this;
  }

  BSTConstIterator operator++(int) {
    BSTConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const BSTConstIterator& rhs) const {
    return current_ == rhs.current_;
  }

  bool operator!=(const BSTConstIterator& rhs) const {
    return current_ != rhs.current_;
  }

  Node<Key, Value>* getNode() const {
    return current_;
  }

protected:
  Node<Key, Value>* current_;
};

template<class Key, class Value>
class BSTIterator : public BSTConstIterator<Key, Value> {
public:
  using value_type = std::pair<const Key, Value>;
  using pointer = value_type*;
  using reference = value_type&;

  BSTIterator(Node<Key, Value>* node = nullptr)
    : BSTConstIterator<Key, Value>(node) {}

  reference operator*() const {
    return this->current_->data_;
  }

  pointer operator->() const {
    return &(this->current_->data_);
  }

  BSTIterator& operator++() {
    BSTConstIterator<Key, Value>::operator++();
    return *this;
  }

  BSTIterator operator++(int) {
    BSTIterator tmp = *this;
    ++(*this);
    return tmp;
  }
};

template<class Key, class Value, class Compare = std::less<Key>>
class BSTree {
public:
  using const_iterator = BSTConstIterator<Key, Value>;
  using iterator = BSTIterator<Key, Value>;

  BSTree() : root_(nullptr), size_(0), cmp_() {}

  ~BSTree() {
    clearTree(root_);
  }

  BSTree(const BSTree& rhs) : root_(nullptr), size_(0), cmp_(rhs.cmp_) {
    for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) {
      push(it->first, it->second);
    }
  }

  BSTree& operator=(const BSTree& rhs) {
    if (this != &rhs) {
      BSTree tmp(rhs);
      std::swap(root_, tmp.root_);
      std::swap(size_, tmp.size_);
      std::swap(cmp_, tmp.cmp_);
    }
    return *this;
  }

  void push(Key k, Value v) {
    if (!root_) {
      root_ = new Node<Key, Value>(k, v);
      ++size_;
      return;
    }
    Node<Key, Value>* curr = root_;
    Node<Key, Value>* parent = nullptr;
    while (curr) {
      parent = curr;
      if (cmp_(k, curr->data_.first)) {
        curr = curr->left_;
      } else if (cmp_(curr->data_.first, k)) {
        curr = curr->right_;
      } else {
        curr->data_.second = v;
        return;
      }
    }
    Node<Key, Value>* newNode = new Node<Key, Value>(k, v, parent);
    if (cmp_(k, parent->data_.first)) {
      parent->left_ = newNode;
    } else {
      parent->right_ = newNode;
    }
    ++size_;
  }

  bool has(Key k) const {
    return findNode(k) != nullptr;
  }

  Value get(Key k) const {
    Node<Key, Value>* node = findNode(k);
    if (!node) {
      throw std::out_of_range("Key not found");
    }
    return node->data_.second;
  }

  Value drop(Key k) {
    Node<Key, Value>* z = findNode(k);
    if (!z) {
      throw std::out_of_range("Key not found");
    }
    Value result = z->data_.second;

    if (!z->left_) {
      transplant(z, z->right_);
    } else if (!z->right_) {
      transplant(z, z->left_);
    } else {
      Node<Key, Value>* y = minNode(z->right_);
      if (y->parent_ != z) {
        transplant(y, y->right_);
        y->right_ = z->right_;
        y->right_->parent_ = y;
      }
      transplant(z, y);
      y->left_ = z->left_;
      y->left_->parent_ = y;
    }
    delete z;
    --size_;
    return result;
  }

  const_iterator rotateLeft(const_iterator it) {
    Node<Key, Value>* x = it.getNode();
    if (!x || !x->right_) {
      return it;
    }
    Node<Key, Value>* y = x->right_;
    x->right_ = y->left_;
    if (y->left_) {
      y->left_->parent_ = x;
    }
    y->parent_ = x->parent_;
    if (!x->parent_) {
      root_ = y;
    } else if (x == x->parent_->left_) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }
    y->left_ = x;
    x->parent_ = y;
    return const_iterator(y);
  }

  const_iterator rotateRight(const_iterator it) {
    Node<Key, Value>* y = it.getNode();
    if (!y || !y->left_) {
      return it;
    }
    Node<Key, Value>* x = y->left_;
    y->left_ = x->right_;
    if (x->right_) {
      x->right_->parent_ = y;
    }
    x->parent_ = y->parent_;
    if (!y->parent_) {
      root_ = x;
    } else if (y == y->parent_->left_) {
      y->parent_->left_ = x;
    } else {
      y->parent_->right_ = x;
    }
    x->right_ = y;
    y->parent_ = x;
    return const_iterator(x);
  }

  size_t height() const {
    return nodeHeight(root_);
  }

  iterator begin() {
    return iterator(minNode(root_));
  }

  iterator end() {
    return iterator(nullptr);
  }

  const_iterator cbegin() const {
    return const_iterator(minNode(root_));
  }

  const_iterator cend() const {
    return const_iterator(nullptr);
  }

  bool empty() const {
    return size_ == 0;
  }

private:
  Node<Key, Value>* root_;
  size_t size_;
  Compare cmp_;

  void clearTree(Node<Key, Value>* node) {
    if (node) {
      clearTree(node->left_);
      clearTree(node->right_);
      delete node;
    }
  }

  Node<Key, Value>* minNode(Node<Key, Value>* node) const {
    if (!node) return nullptr;
    while (node->left_) {
      node = node->left_;
    }
    return node;
  }

  Node<Key, Value>* findNode(Key k) const {
    Node<Key, Value>* curr = root_;
    while (curr) {
      if (cmp_(k, curr->data_.first)) {
        curr = curr->left_;
      } else if (cmp_(curr->data_.first, k)) {
        curr = curr->right_;
      } else {
        return curr;
      }
    }
    return nullptr;
  }

  void transplant(Node<Key, Value>* u, Node<Key, Value>* v) {
    if (!u->parent_) {
      root_ = v;
    } else if (u == u->parent_->left_) {
      u->parent_->left_ = v;
    } else {
      u->parent_->right_ = v;
    }
    if (v) {
      v->parent_ = u->parent_;
    }
  }

  size_t nodeHeight(Node<Key, Value>* node) const {
    if (!node) return 0;
    size_t lh = nodeHeight(node->left_);
    size_t rh = nodeHeight(node->right_);
    return 1 + (lh > rh ? lh : rh);
  }
};

}

#endif
