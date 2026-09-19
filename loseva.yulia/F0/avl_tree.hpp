#ifndef LOSEVA_AVL_TREE_HPP
#define LOSEVA_AVL_TREE_HPP

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

namespace loseva {

template< class T >
struct Node {
  T data_;
  int height_;
  Node* left_;
  Node* right_;
  Node* parent_;

  explicit Node(const T & val, Node* p = nullptr) noexcept:
    data_(val),
    height_(1),
    left_(nullptr),
    right_(nullptr),
    parent_(p)
  {}
};

template< class T >
class AVLConstIterator {
public:
  using value_type = const T;
  using pointer = const T *;
  using reference = const T &;

  AVLConstIterator(Node< T > * node = nullptr) noexcept:
    current_(node)
  {}

  reference operator*() const noexcept
  {
    return current_->data_;
  }

  pointer operator->() const noexcept
  {
    return &(current_->data_);
  }

  AVLConstIterator & operator++() noexcept
  {
    if (current_->right_) {
      current_ = current_->right_;
      while (current_->left_) {
        current_ = current_->left_;
      }
    } else {
      Node< T > * p = current_->parent_;
      while (p && current_ == p->right_) {
        current_ = p;
        p = p->parent_;
      }
      current_ = p;
    }
    return *this;
  }

  AVLConstIterator operator++(int) noexcept
  {
    AVLConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const AVLConstIterator & rhs) const noexcept
  {
    return current_ == rhs.current_;
  }

  bool operator!=(const AVLConstIterator & rhs) const noexcept
  {
    return current_ != rhs.current_;
  }

  Node< T > * getNode() const noexcept
  {
    return current_;
  }

protected:
  Node< T > * current_;
};

template< class T >
class AVLTree {
public:
  using const_iterator = AVLConstIterator< T >;

  AVLTree() noexcept:
    root_(nullptr),
    size_(0)
  {}

  ~AVLTree() noexcept
  {
    clear();
  }

  AVLTree(const AVLTree & rhs):
    root_(nullptr),
    size_(0)
  {
    for (auto it = rhs.cbegin(); it != rhs.cend(); ++it) {
      insert(*it);
    }
  }

  AVLTree(AVLTree && rhs) noexcept:
    root_(rhs.root_),
    size_(rhs.size_)
  {
    rhs.root_ = nullptr;
    rhs.size_ = 0;
  }

  AVLTree & operator=(const AVLTree & rhs)
  {
    if (this != &rhs) {
      AVLTree tmp(rhs);
      std::swap(root_, tmp.root_);
      std::swap(size_, tmp.size_);
    }
    return *this;
  }

  AVLTree & operator=(AVLTree && rhs) noexcept
  {
    if (this != &rhs) {
      clear();
      root_ = rhs.root_;
      size_ = rhs.size_;
      rhs.root_ = nullptr;
      rhs.size_ = 0;
    }
    return *this;
  }

  bool insert(const T & val)
  {
    if (has(val)) {
      return false;
    }
    root_ = insertRec(root_, val, nullptr);
    ++size_;
    return true;
  }

  bool remove(const T & val)
  {
    if (!has(val)) {
      return false;
    }
    root_ = removeRec(root_, val);
    --size_;
    return true;
  }

  bool has(const T & val) const noexcept
  {
    return findNode(val) != nullptr;
  }

  T * find(const T & val) noexcept
  {
    Node< T > * node = findNode(val);
    return node ? &(node->data_) : nullptr;
  }

  const T * find(const T & val) const noexcept
  {
    Node< T > * node = findNode(val);
    return node ? &(node->data_) : nullptr;
  }

  void clear() noexcept
  {
    while (root_ != nullptr) {
      if (root_->left_ != nullptr) {
        Node< T > * leftChild = root_->left_;
        root_->left_ = leftChild->right_;
        if (leftChild->right_) {
          leftChild->right_->parent_ = root_;
        }
        leftChild->right_ = root_;
        root_->parent_ = leftChild;
        root_ = leftChild;
        root_->parent_ = nullptr;
      } else {
        Node< T > * temp = root_;
        root_ = root_->right_;
        if (root_) {
          root_->parent_ = nullptr;
        }
        delete temp;
      }
    }
    size_ = 0;
  }

  int height() const noexcept
  {
    return height(root_);
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  void rotateLeftManual(const T & val)
  {
    Node< T > * x = findNode(val);
    if (!x || !x->right_) {
      throw std::invalid_argument("Cannot rotate left");
    }
    Node< T > * y = x->right_;
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
    updateHeightsUpward(x);
  }

  void rotateRightManual(const T & val)
  {
    Node< T > * y = findNode(val);
    if (!y || !y->left_) {
      throw std::invalid_argument("Cannot rotate right");
    }
    Node< T > * x = y->left_;
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
    updateHeightsUpward(y);
  }

  const_iterator cbegin() const noexcept
  {
    return const_iterator(minNode(root_));
  }

  const_iterator cend() const noexcept
  {
    return const_iterator(nullptr);
  }

  void printTreeShape(std::ostream & out) const
  {
    if (!root_) {
      out << "<EMPTY>\n";
      return;
    }
    printShapeRec(root_, "", false, true, out);
  }

private:
  Node< T > * root_;
  std::size_t size_;

  int height(Node< T > * p) const noexcept
  {
    return p ? p->height_ : 0;
  }

  int bfactor(Node< T > * p) const noexcept
  {
    return p ? height(p->right_) - height(p->left_) : 0;
  }

  void fixHeight(Node< T > * p) noexcept
  {
    if (p) {
      int hl = height(p->left_);
      int hr = height(p->right_);
      p->height_ = (hl > hr ? hl : hr) + 1;
    }
  }

  void updateHeightsUpward(Node< T > * p) noexcept
  {
    while (p) {
      fixHeight(p);
      p = p->parent_;
    }
  }

  Node< T > * rotateRight(Node< T > * p) noexcept
  {
    Node< T > * q = p->left_;
    p->left_ = q->right_;
    if (q->right_) {
      q->right_->parent_ = p;
    }
    q->parent_ = p->parent_;
    q->right_ = p;
    p->parent_ = q;
    fixHeight(p);
    fixHeight(q);
    return q;
  }

  Node< T > * rotateLeft(Node< T > * q) noexcept
  {
    Node< T > * p = q->right_;
    q->right_ = p->left_;
    if (p->left_) {
      p->left_->parent_ = q;
    }
    p->parent_ = q->parent_;
    p->left_ = q;
    q->parent_ = p;
    fixHeight(q);
    fixHeight(p);
    return p;
  }

  Node< T > * balance(Node< T > * p) noexcept
  {
    fixHeight(p);
    if (bfactor(p) == 2) {
      if (bfactor(p->right_) < 0) {
        p->right_ = rotateRight(p->right_);
        if (p->right_) p->right_->parent_ = p;
      }
      return rotateLeft(p);
    }
    if (bfactor(p) == -2) {
      if (bfactor(p->left_) > 0) {
        p->left_ = rotateLeft(p->left_);
        if (p->left_) p->left_->parent_ = p;
      }
      return rotateRight(p);
    }
    return p;
  }

  Node< T > * insertRec(Node< T > * p, const T & k, Node< T > * parent)
  {
    if (!p) {
      return new Node< T >(k, parent);
    }
    if (k < p->data_) {
      p->left_ = insertRec(p->left_, k, p);
    } else {
      p->right_ = insertRec(p->right_, k, p);
    }
    return balance(p);
  }

  Node< T > * findMin(Node< T > * p) const noexcept
  {
    return p->left_ ? findMin(p->left_) : p;
  }

  Node< T > * removeMin(Node< T > * p)
  {
    if (!p->left_) {
      return p->right_;
    }
    p->left_ = removeMin(p->left_);
    if (p->left_) p->left_->parent_ = p;
    return balance(p);
  }

  Node< T > * removeRec(Node< T > * p, const T & k)
  {
    if (!p) {
      return nullptr;
    }
    if (k < p->data_) {
      p->left_ = removeRec(p->left_, k);
      if (p->left_) p->left_->parent_ = p;
    } else if (k > p->data_) {
      p->right_ = removeRec(p->right_, k);
      if (p->right_) p->right_->parent_ = p;
    } else {
      Node< T > * q = p->left_;
      Node< T > * r = p->right_;
      Node< T > * par = p->parent_;
      delete p;
      if (!r) {
        if (q) q->parent_ = par;
        return q;
      }
      Node< T > * min = findMin(r);
      min->right_ = removeMin(r);
      if (min->right_) min->right_->parent_ = min;
      min->left_ = q;
      if (min->left_) min->left_->parent_ = min;
      min->parent_ = par;
      return balance(min);
    }
    return balance(p);
  }

  Node< T > * findNode(const T & val) const noexcept
  {
    Node< T > * curr = root_;
    while (curr) {
      if (val < curr->data_) {
        curr = curr->left_;
      } else if (val > curr->data_) {
        curr = curr->right_;
      } else {
        return curr;
      }
    }
    return nullptr;
  }

  Node< T > * minNode(Node< T > * p) const noexcept
  {
    if (!p) return nullptr;
    while (p->left_) {
      p = p->left_;
    }
    return p;
  }

  void printShapeRec(
    Node< T > * node,
    std::string prefix,
    bool isLeft,
    bool isRoot,
    std::ostream & out) const
  {
    if (!node) {
      return;
    }
    printShapeRec(node->right_, prefix + (isLeft ? "│   " : "    "), false, false, out);
    out << prefix;
    if (!isRoot) {
      out << (isLeft ? "└── " : "┌── ");
    } else {
      out << "└── ";
    }
    out << node->data_ << "\n";
    printShapeRec(node->left_, prefix + (isLeft ? "    " : "│   "), true, false, out);
  }
};

}

#endif
