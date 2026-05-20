#ifndef KARPENKO_AVL_TREE_HPP
#define KARPENKO_AVL_TREE_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace karpenko
{

template < typename Key, typename Value >
struct AVLNode
{
  std::pair< const Key, Value > data_;
  AVLNode* left_;
  AVLNode* right_;
  AVLNode* parent_;
  int height_;

  AVLNode(const Key& k, const Value& v)
    : data_(k, v),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
  {
  }

  AVLNode(const Key& k, Value&& v)
    : data_(k, std::move(v)),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr),
      height_(1)
  {
  }
};

template < typename Key, typename Value, typename Compare = std::less< Key > >
class AVLTree
{
public:
  using Node = AVLNode< Key, Value >;

private:
  Node* root_;
  Compare cmp_;

  static int nodeHeight(const Node* n)
  {
    return n ? n->height_ : 0;
  }

  void updateHeight(Node* n)
  {
    if (n)
    {
      n->height_ = 1 + std::max(nodeHeight(n->left_), nodeHeight(n->right_));
    }
  }

  int balanceFactor(const Node* n) const
  {
    return nodeHeight(n->right_) - nodeHeight(n->left_);
  }

  Node* rotateRight(Node* y)
  {
    Node* x = y->left_;
    Node* t2 = x->right_;

    x->right_ = y;
    y->left_ = t2;

    if (t2)
    {
      t2->parent_ = y;
    }
    x->parent_ = y->parent_;
    y->parent_ = x;

    updateHeight(y);
    updateHeight(x);
    return x;
  }

  Node* rotateLeft(Node* x)
  {
    Node* y = x->right_;
    Node* t2 = y->left_;

    y->left_ = x;
    x->right_ = t2;

    if (t2)
    {
      t2->parent_ = x;
    }
    y->parent_ = x->parent_;
    x->parent_ = y;

    updateHeight(x);
    updateHeight(y);
    return y;
  }

  Node* balance(Node* n)
  {
    if (!n)
    {
      return nullptr;
    }
    updateHeight(n);
    const int bf = balanceFactor(n);
    if (bf == 2)
    {
      if (balanceFactor(n->right_) < 0)
      {
        n->right_ = rotateRight(n->right_);
      }
      return rotateLeft(n);
    }
    if (bf == -2)
    {
      if (balanceFactor(n->left_) > 0)
      {
        n->left_ = rotateLeft(n->left_);
      }
      return rotateRight(n);
    }
    return n;
  }

  Node* insert(Node* n, const Key& k, const Value& v, Node* parent)
  {
    if (!n)
    {
      Node* node = new Node(k, v);
      node->parent_ = parent;
      return node;
    }
    if (cmp_(k, n->data_.first))
    {
      n->left_ = insert(n->left_, k, v, n);
    }
    else if (cmp_(n->data_.first, k))
    {
      n->right_ = insert(n->right_, k, v, n);
    }
    else
    {
      n->data_.second = v;
    }
    return balance(n);
  }

  Node* findNode(Node* n, const Key& k)
  {
    if (!n)
    {
      return nullptr;
    }
    if (cmp_(k, n->data_.first))
    {
      return findNode(n->left_, k);
    }
    if (cmp_(n->data_.first, k))
    {
      return findNode(n->right_, k);
    }
    return n;
  }

  const Node* findNode(const Node* n, const Key& k) const
  {
    if (!n)
    {
      return nullptr;
    }
    if (cmp_(k, n->data_.first))
    {
      return findNode(n->left_, k);
    }
    if (cmp_(n->data_.first, k))
    {
      return findNode(n->right_, k);
    }
    return n;
  }

  Node* minimum(Node* n) const
  {
    while (n && n->left_)
    {
      n = n->left_;
    }
    return n;
  }

  Node* erase(Node* n, const Key& k)
  {
    if (!n)
    {
      return nullptr;
    }
    if (cmp_(k, n->data_.first))
    {
      n->left_ = erase(n->left_, k);
    }
    else if (cmp_(n->data_.first, k))
    {
      n->right_ = erase(n->right_, k);
    }
    else
    {
      if (!n->left_ || !n->right_)
      {
        Node* child = n->left_ ? n->left_ : n->right_;
        if (child)
        {
          child->parent_ = n->parent_;
        }
        delete n;
        return child;
      }
      else
      {
        Node* successor = minimum(n->right_);
        const_cast< Key& >(n->data_.first) = successor->data_.first;
        n->data_.second = std::move(successor->data_.second);
        n->right_ = erase(n->right_, successor->data_.first);
      }
    }
    return balance(n);
  }

  void clear(Node* n)
  {
    if (!n)
    {
      return;
    }
    clear(n->left_);
    clear(n->right_);
    delete n;
  }

  Node* copyNode(Node* n, Node* parent)
  {
    if (!n)
    {
      return nullptr;
    }
    Node* newNode = new Node(n->data_.first, n->data_.second);
    newNode->parent_ = parent;
    newNode->left_ = copyNode(n->left_, newNode);
    newNode->right_ = copyNode(n->right_, newNode);
    newNode->height_ = n->height_;
    return newNode;
  }

public:
  AVLTree()
    : root_(nullptr),
      cmp_()
  {
  }

  ~AVLTree()
  {
    clear(root_);
  }

  AVLTree(const AVLTree& other)
    : root_(nullptr),
      cmp_(other.cmp_)
  {
    root_ = copyNode(other.root_, nullptr);
  }

  AVLTree& operator=(const AVLTree& other)
  {
    if (this != &other)
    {
      clear(root_);
      cmp_ = other.cmp_;
      root_ = copyNode(other.root_, nullptr);
    }
    return *this;
  }

  AVLTree(AVLTree&& other) noexcept
    : root_(other.root_),
      cmp_(std::move(other.cmp_))
  {
    other.root_ = nullptr;
  }

  AVLTree& operator=(AVLTree&& other) noexcept
  {
    if (this != &other)
    {
      clear(root_);
      root_ = other.root_;
      cmp_ = std::move(other.cmp_);
      other.root_ = nullptr;
    }
    return *this;
  }

  void insert(const Key& k, const Value& v)
  {
    root_ = insert(root_, k, v, nullptr);
  }

  Value& at(const Key& k)
  {
    Node* n = findNode(root_, k);
    if (!n)
    {
      throw std::out_of_range("key not found");
    }
    return n->data_.second;
  }

  const Value& at(const Key& k) const
  {
    const Node* n = findNode(root_, k);
    if (!n)
    {
      throw std::out_of_range("key not found");
    }
    return n->data_.second;
  }

  bool contains(const Key& k) const
  {
    return findNode(root_, k) != nullptr;
  }

  void erase(const Key& k)
  {
    root_ = erase(root_, k);
  }

  size_t getHeight() const
  {
    return nodeHeight(root_);
  }

  class const_iterator
  {
  private:
    const Node* ptr_;
    const Node* root_;

    void advance()
    {
      if (ptr_->right_)
      {
        ptr_ = ptr_->right_;
        while (ptr_->left_)
        {
          ptr_ = ptr_->left_;
        }
        return;
      }
      const Node* p = ptr_->parent_;
      while (p && ptr_ == p->right_)
      {
        ptr_ = p;
        p = p->parent_;
      }
      ptr_ = p;
    }

  public:
    const_iterator(const Node* p = nullptr, const Node* r = nullptr)
      : ptr_(p),
        root_(r)
    {
    }

    const std::pair< const Key, Value >& operator*() const
    {
      return ptr_->data_;
    }

    const std::pair< const Key, Value >* operator->() const
    {
      return &ptr_->data_;
    }

    const_iterator& operator++()
    {
      advance();
      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator tmp = *this;
      advance();
      return tmp;
    }

    bool operator==(const const_iterator& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const const_iterator& other) const
    {
      return !(*this == other);
    }
  };

  const_iterator begin() const
  {
    Node* cur = root_;
    while (cur && cur->left_)
    {
      cur = cur->left_;
    }
    return const_iterator(cur, root_);
  }

  const_iterator end() const
  {
    return const_iterator(nullptr, root_);
  }

  size_t size() const
  {
    size_t cnt = 0;
    for (auto it = begin(); it != end(); ++it)
    {
      ++cnt;
    }
    return cnt;
  }

  bool empty() const
  {
    return root_ == nullptr;
  }
};

}

#endif
