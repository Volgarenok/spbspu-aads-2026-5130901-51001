#ifndef LOSEV_AVLTREE_HPP
#define LOSEV_AVLTREE_HPP

#include <cstddef>
#include <stdexcept>
#include <functional>
#include <utility>
#include <algorithm>

namespace losev {

template <typename Key, typename Value, typename Compare = std::less<Key>>
class AVLTree
{
private:
  struct Node
  {
    Key key;
    Value value;
    Node* left;
    Node* right;
    Node* parent;
    size_t height;

    Node(const Key& k, const Value& v, Node* p = nullptr)
      : key(k), value(v), left(nullptr), right(nullptr), parent(p), height(1)
    {}
  };

  Node* root_;
  size_t size_;
  Compare comp_;

  size_t getHeight(Node* node) const
  {
    return node == nullptr ? 0 : node->height;
  }

  int getBalance(Node* node) const
  {
    return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
  }

  void updateHeight(Node* node)
  {
    if (node != nullptr)
    {
      node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
    }
  }

  Node* rotateLeft(Node* x)
  {
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

    updateHeight(x);
    updateHeight(y);

    return y;
  }

  Node* rotateRight(Node* y)
  {
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

    updateHeight(y);
    updateHeight(x);

    return x;
  }

  Node* balance(Node* node)
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    updateHeight(node);
    int balance = getBalance(node);

    if (balance > 1)
    {
      if (getBalance(node->left) < 0)
      {
        node->left = rotateLeft(node->left);
      }
      return rotateRight(node);
    }
    if (balance < -1)
    {
      if (getBalance(node->right) > 0)
      {
        node->right = rotateRight(node->right);
      }
      return rotateLeft(node);
    }

    return node;
  }

  Node* findMin(Node* node) const
  {
    if (node == nullptr) return nullptr;
    while (node->left != nullptr) node = node->left;
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

  Node* insert(Node* node, Node* parent, const Key& k, const Value& v)
  {
    if (node == nullptr)
    {
      Node* newNode = new Node(k, v, parent);
      ++size_;
      return newNode;
    }

    if (comp_(k, node->key))
    {
      node->left = insert(node->left, node, k, v);
    }
    else if (comp_(node->key, k))
    {
      node->right = insert(node->right, node, k, v);
    }
    else
    {
      node->value = v;
      return node;
    }

    return balance(node);
  }

  Node* remove(Node* node, const Key& k)
  {
    if (node == nullptr)
    {
      return nullptr;
    }

    if (comp_(k, node->key))
    {
      node->left = remove(node->left, k);
    }
    else if (comp_(node->key, k))
    {
      node->right = remove(node->right, k);
    }
    else
    {
      if (node->left == nullptr || node->right == nullptr)
      {
        Node* temp = node->left != nullptr ? node->left : node->right;

        if (temp == nullptr)
        {
          temp = node;
          node = nullptr;
        }
        else
        {
          temp->parent = node->parent;
          *node = *temp;
        }

        delete temp;
        --size_;
      }
      else
      {
        Node* successor = findMin(node->right);
        node->key = successor->key;
        node->value = successor->value;
        node->right = remove(node->right, successor->key);
      }
    }

    if (node == nullptr)
    {
      return nullptr;
    }

    return balance(node);
  }

  void clearRecursive(Node* node)
  {
    if (node == nullptr) return;
    clearRecursive(node->left);
    clearRecursive(node->right);
    delete node;
  }

  void copyRecursive(Node*& dest, Node* src, Node* parent)
  {
    if (src == nullptr)
    {
      dest = nullptr;
      return;
    }

    dest = new Node(src->key, src->value, parent);
    dest->height = src->height;

    copyRecursive(dest->left, src->left, dest);
    copyRecursive(dest->right, src->right, dest);
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
        Node* current = root_;
        while (current != nullptr && current->right != nullptr)
        {
          current = current->right;
        }
        ptr_ = current;
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
        const Node* current = root_;
        while (current != nullptr && current->right != nullptr)
        {
          current = current->right;
        }
        ptr_ = current;
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

  AVLTree() : root_(nullptr), size_(0) {}

  ~AVLTree()
  {
    clearRecursive(root_);
  }

  AVLTree(const AVLTree& other)
    : root_(nullptr), size_(0), comp_(other.comp_)
  {
    copyRecursive(root_, other.root_, nullptr);
    size_ = other.size_;
  }

  AVLTree(AVLTree&& other) noexcept
    : root_(other.root_), size_(other.size_), comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  AVLTree& operator=(const AVLTree& other)
  {
    if (this != &other)
    {
      clearRecursive(root_);
      copyRecursive(root_, other.root_, nullptr);
      size_ = other.size_;
      comp_ = other.comp_;
    }
    return *this;
  }

  AVLTree& operator=(AVLTree&& other) noexcept
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
    root_ = insert(root_, nullptr, k, v);
  }

  const Value& get(const Key& k) const
  {
    Node* current = findNode(k);
    if (current == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return current->value;
  }

  Value& get(const Key& k)
  {
    Node* current = findNode(k);
    if (current == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return current->value;
  }

  Value drop(const Key& k)
  {
    Node* target = findNode(k);
    if (target == nullptr)
    {
      throw std::out_of_range("key not found");
    }

    Value result = std::move(target->value);
    root_ = remove(root_, k);
    return result;
  }

  size_t height() const
  {
    return getHeight(root_);
  }

  iterator begin()
  {
    Node* min = root_;
    while (min != nullptr && min->left != nullptr)
    {
      min = min->left;
    }
    return iterator(min);
  }

  iterator end()
  {
    return iterator(nullptr);
  }

  const_iterator begin() const
  {
    Node* min = root_;
    while (min != nullptr && min->left != nullptr)
    {
      min = min->left;
    }
    return const_iterator(min);
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
