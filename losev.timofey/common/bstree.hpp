#ifndef LOSEV_BSTREE_HPP
#define LOSEV_BSTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>
#include <algorithm>

namespace losev {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class BSTree
{
private:
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;

    Node(const Key& k, const Value& v, Node* p = nullptr)
      : key(k), value(v), left(nullptr), right(nullptr), parent(p)
    {}
  };

  Node* root_;
  size_t size_;
  Compare comp_;

  Node* findMin(Node* node) const
  {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) node = node->left;
    return node;
  }

  Node* findMax(Node* node) const
  {
    if (node == nullptr) return nullptr;
    while (node->right != nullptr) node = node->right;
    return node;
  }

  Node* findNode(const Key& k) const
  {
    Node* current = root_;
    while (current != nullptr)
    {
      if (comp_(k, current->key))
      {
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        current = current->right;
      }
      else
      {
        return current;
      }
    }
    return nullptr;
  }

  void transplant(Node* u, Node* v)
  {
    if (u->parent == nullptr)
    {
      root_ = v;
    }
    else if (u == u->parent->left)
    {
      u->parent->left = v;
    }
    else
    {
      u->parent->right = v;
    }
    if (v != nullptr)
    {
      v->parent = u->parent;
    }
  }

  size_t heightRecursive(Node* node) const
  {
    if (node == nullptr)
    {
      return 0;
    }
    return 1 + std::max(heightRecursive(node->left), heightRecursive(node->right));
  }

  void clearRecursive(Node* node)
  {
    if (node == nullptr) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
  }

public:
  class Iterator
  {
  private:
    Node* ptr_;

    void goToNext()
    {
      if (ptr_->right != nullptr)
      {
        ptr_ = ptr_->right;
        while (ptr_->left != nullptr) ptr_ = ptr_->left;
      }
      else
      {
        Node* parent = ptr_->parent;
        while (parent != nullptr && ptr_ == parent->right)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

    void goToPrev()
    {
      if (ptr_->left != nullptr)
      {
        ptr_ = ptr_->left;
        while (ptr_->right != nullptr) ptr_ = ptr_->right;
      }
      else
      {
        Node* parent = ptr_->parent;
        while (parent != nullptr && ptr_ == parent->left)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

  public:
    explicit Iterator(Node* ptr = nullptr) : ptr_(ptr) {}

    bool operator==(const Iterator& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const Iterator& other) const
    {
      return ptr_ != other.ptr_;
    }

    std::pair<const Key, Value&> operator*()
    {
      return std::pair<const Key, Value&>(ptr_->key, ptr_->value);
    }

    Iterator& operator++()
    {
      goToNext();
      return *this;
    }

    Iterator operator++(int)
    {
      Iterator tmp = *this;
      goToNext();
      return tmp;
    }

    Iterator& operator--()
    {
      if (ptr_ == nullptr)
      {
        ptr_ = findMax(root_);
      }
      else
      {
        goToPrev();
      }
      return *this;
    }

    Iterator operator--(int)
    {
      Iterator tmp = *this;
      --(*this);
      return tmp;
    }
  };

  class ConstIterator
  {
  private:
    const Node* ptr_;

    void goToNext()
    {
      if (ptr_->right != nullptr)
      {
        ptr_ = ptr_->right;
        while (ptr_->left != nullptr) ptr_ = ptr_->left;
      }
      else
      {
        const Node* parent = ptr_->parent;
        while (parent != nullptr && ptr_ == parent->right)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

    void goToPrev()
    {
      if (ptr_->left != nullptr)
      {
        ptr_ = ptr_->left;
        while (ptr_->right != nullptr) ptr_ = ptr_->right;
      }
      else
      {
        const Node* parent = ptr_->parent;
        while (parent != nullptr && ptr_ == parent->left)
        {
          ptr_ = parent;
          parent = parent->parent;
        }
        ptr_ = parent;
      }
    }

  public:
    explicit ConstIterator(const Node* ptr = nullptr) : ptr_(ptr) {}

    ConstIterator(const Iterator& other) : ptr_(other.ptr_) {}

    bool operator==(const ConstIterator& other) const
    {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const ConstIterator& other) const
    {
      return ptr_ != other.ptr_;
    }

    std::pair<const Key, const Value&> operator*() const
    {
      return std::pair<const Key, const Value&>(ptr_->key, ptr_->value);
    }

    ConstIterator& operator++()
    {
      goToNext();
      return *this;
    }

    ConstIterator operator++(int)
    {
      ConstIterator tmp = *this;
      goToNext();
      return tmp;
    }

    ConstIterator& operator--()
    {
      if (ptr_ == nullptr)
      {
        ptr_ = findMax(root_);
      }
      else
      {
        goToPrev();
      }
      return *this;
    }

    ConstIterator operator--(int)
    {
      ConstIterator tmp = *this;
      --(*this);
      return tmp;
    }
  };

  using iterator = Iterator;
  using const_iterator = ConstIterator;

  BSTree() : root_(nullptr), size_(0) {}

  ~BSTree()
  {
    clearRecursive(root_);
  }

  BSTree(const BSTree& other)
    : root_(nullptr)
    , size_(0)
    , comp_(other.comp_)
  {
    for (auto it = other.cbegin(); it != other.cend(); ++it)
    {
      auto pair = *it;
      push(pair.first, pair.second);
    }
  }

  BSTree(BSTree&& other) noexcept
    : root_(other.root_)
    , size_(other.size_)
    , comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  BSTree& operator=(const BSTree& other)
  {
    if (this != &other)
    {
      BSTree tmp(other);
      std::swap(root_, tmp.root_);
      std::swap(size_, tmp.size_);
      std::swap(comp_, tmp.comp_);
    }
    return *this;
  }

  BSTree& operator=(BSTree&& other) noexcept
  {
    if (this != &other)
    {
      clearRecursive(root_);
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  bool empty() const { return size_ == 0; }
  size_t size() const { return size_; }

  void push(const Key& k, const Value& v)
  {
    if (root_ == nullptr)
    {
      root_ = new Node(k, v);
      ++size_;
      return;
    }

    Node* current = root_;
    Node* parent = nullptr;
    while (current != nullptr)
    {
      parent = current;
      if (comp_(k, current->key))
      {
        current = current->left;
      }
      else if (comp_(current->key, k))
      {
        current = current->right;
      }
      else
      {
        current->value = v;
        return;
      }
    }

    Node* newNode = new Node(k, v, parent);
    if (comp_(k, parent->key))
    {
      parent->left = newNode;
    }
    else
    {
      parent->right = newNode;
    }
    ++size_;
  }

  const Value& get(const Key& k) const
  {
    Node* current = root_;
    while (current != nullptr)
    {
      if (comp_(k, current->key)) current = current->left;
      else if (comp_(current->key, k)) current = current->right;
      else return current->value;
    }
    throw std::out_of_range("key not found");
  }

  Value& get(const Key& k)
  {
    Node* current = root_;
    while (current != nullptr)
    {
      if (comp_(k, current->key)) current = current->left;
      else if (comp_(current->key, k)) current = current->right;
      else return current->value;
    }
    throw std::out_of_range("key not found");
  }

  Value drop(const Key& k)
  {
    Node* target = findNode(k);
    if (target == nullptr)
    {
      throw std::out_of_range("key not found");
    }

    Value result = std::move(target->value);

    if (target->left == nullptr)
    {
      transplant(target, target->right);
    }
    else if (target->right == nullptr)
    {
      transplant(target, target->left);
    }
    else
    {
      Node* successor = findMin(target->right);
      if (successor->parent != target)
      {
        transplant(successor, successor->right);
        successor->right = target->right;
        successor->right->parent = successor;
      }
      transplant(target, successor);
      successor->left = target->left;
      successor->left->parent = successor;
    }

    delete target;
    --size_;
    return result;
  }

  size_t height() const
  {
    return heightRecursive(root_);
  }

  const_iterator rotateLeft(const_iterator it)
  {
    Node* x = const_cast<Node*>(it.ptr_);
    if (x == nullptr || x->right == nullptr)
    {
      return it;
    }

    Node* y = x->right;
    Node* parent = x->parent;

    x->right = y->left;
    if (y->left != nullptr)
    {
      y->left->parent = x;
    }

    y->left = x;
    x->parent = y;
    y->parent = parent;

    if (parent == nullptr)
    {
      root_ = y;
    }
    else if (parent->left == x)
    {
      parent->left = y;
    }
    else
    {
      parent->right = y;
    }

    return const_iterator(y);
  }

  const_iterator rotateRight(const_iterator it)
  {
    Node* y = const_cast<Node*>(it.ptr_);
    if (y == nullptr || y->left == nullptr)
    {
      return it;
    }

    Node* x = y->left;
    Node* parent = y->parent;

    y->left = x->right;
    if (x->right != nullptr)
    {
      x->right->parent = y;
    }

    x->right = y;
    y->parent = x;
    x->parent = parent;

    if (parent == nullptr)
    {
      root_ = x;
    }
    else if (parent->left == y)
    {
      parent->left = x;
    }
    else
    {
      parent->right = x;
    }

    return const_iterator(x);
  }

  const_iterator rotateLargeLeft(const_iterator it)
  {
    Node* x = const_cast<Node*>(it.ptr_);
    if (x == nullptr || x->right == nullptr)
    {
      return it;
    }

    Node* y = x->right;
    if (y->left == nullptr)
    {
      return rotateLeft(it);
    }

    Node* z = y->left;
    Node* parent = x->parent;

    x->right = z->left;
    if (z->left != nullptr)
    {
      z->left->parent = x;
    }

    y->left = z->right;
    if (z->right != nullptr)
    {
      z->right->parent = y;
    }

    z->left = x;
    x->parent = z;
    z->right = y;
    y->parent = z;
    z->parent = parent;

    if (parent == nullptr)
    {
      root_ = z;
    }
    else if (parent->left == x)
    {
      parent->left = z;
    }
    else
    {
      parent->right = z;
    }

    return const_iterator(z);
  }

  const_iterator rotateLargeRight(const_iterator it)
  {
    Node* y = const_cast<Node*>(it.ptr_);
    if (y == nullptr || y->left == nullptr)
    {
      return it;
    }

    Node* x = y->left;
    if (x->right == nullptr)
    {
      return rotateRight(it);
    }

    Node* z = x->right;
    Node* parent = y->parent;

    y->left = z->right;
    if (z->right != nullptr)
    {
      z->right->parent = y;
    }

    x->right = z->left;
    if (z->left != nullptr)
    {
      z->left->parent = x;
    }

    z->right = y;
    y->parent = z;
    z->left = x;
    x->parent = z;
    z->parent = parent;

    if (parent == nullptr)
    {
      root_ = z;
    }
    else if (parent->left == y)
    {
      parent->left = z;
    }
    else
    {
      parent->right = z;
    }

    return const_iterator(z);
  }

  iterator begin()
  {
    return iterator(findMin(root_));
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  const_iterator begin() const
  {
    return const_iterator(findMin(root_));
  }

  const_iterator end() const
  {
    return const_iterator(nullptr);
  }

  const_iterator cbegin() const { return begin(); }
  const_iterator cend() const { return end(); }

  void clear()
  {
    clearRecursive(root_);
    root_ = nullptr;
    size_ = 0;
  }
};

}

#endif
