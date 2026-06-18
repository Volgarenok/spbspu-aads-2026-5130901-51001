#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <iostream>

namespace shevchenko
{

template< class Key, class Value >
class BSTIterator;

template< class Key, class Value >
class BSTConstIterator;

template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
public:
  struct Node
  {
    std::pair< Key, Value > data;
    Node* parent;
    Node* left;
    Node* right;

    Node(const Key& key, const Value& value, Node* par)
    : data(key, value)
    , parent(par)
    , left(nullptr)
    , right(nullptr)
    {}
  };

  using iterator = BSTIterator< Key, Value >;
  using const_iterator = BSTConstIterator< Key, Value >;

  BSTree()
  : fake_(new Node(Key(), Value(), nullptr))
  , size_(0)
  , comp_(Compare())
  {
    fake_->left = nullptr;
    fake_->right = nullptr;
    fake_->parent = nullptr;
  }

  BSTree(const BSTree& other)
  : fake_(new Node(Key(), Value(), nullptr))
  , size_(0)
  , comp_(other.comp_)
  {
    fake_->left = copy(other.fake_->left, fake_);
  }

  BSTree(BSTree&& other) noexcept
  : fake_(other.fake_)
  , size_(other.size_)
  , comp_(std::move(other.comp_))
  {
    other.fake_ = new Node(Key(), Value(), nullptr);
    other.fake_->left = nullptr;
    other.fake_->right = nullptr;
    other.fake_->parent = nullptr;
    other.size_ = 0;
  }

  ~BSTree()
  {
    clear();
    delete fake_;
  }

  BSTree& operator=(const BSTree& other)
  {
    if (this != &other)
    {
      BSTree tmp(other);
      swap(tmp);
    }
    return *this;
  }

  BSTree& operator=(BSTree&& other) noexcept
  {
    if (this != &other)
    {
      clear();
      delete fake_;
      fake_ = other.fake_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.fake_ = new Node(Key(), Value(), nullptr);
      other.fake_->left = nullptr;
      other.fake_->right = nullptr;
      other.fake_->parent = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  void swap(BSTree& other) noexcept
  {
    std::swap(fake_, other.fake_);
    std::swap(size_, other.size_);
  }

  void push(const Key& key, const Value& value)
  {
    Node* parent = fake_;
    Node* current = fake_->left;

    while (current != nullptr)
    {
      parent = current;
      if (comp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (comp_(current->data.first, key))
      {
        current = current->right;
      }
      else
      {
        current->data.second = value;
        return;
      }
    }

    Node* new_node = new Node(key, value, parent);
    if (parent == fake_)
    {
      fake_->left = new_node;
    }
    else if (comp_(key, parent->data.first))
    {
      parent->left = new_node;
    }
    else
    {
      parent->right = new_node;
    }
    ++size_;
  }

  Value& get(const Key& key)
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("no such key");
    }
    return node->data.second;
  }

  const Value& get(const Key& key) const
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("no such key");
    }
    return node->data.second;
  }

  bool has(const Key& key) const
  {
    return findNode(key) != nullptr;
  }

  bool empty() const noexcept
  {
    return size_ == 0;
  }

  size_t size() const noexcept
  {
    return size_;
  }

  void clear()
  {
    clearSubtree(fake_->left);
    fake_->left = nullptr;
    size_ = 0;
  }

  iterator begin()
  {
    return iterator(minimum(fake_->left), fake_);
  }

  iterator end()
  {
    return iterator(nullptr, fake_);
  }

  const_iterator cbegin() const
  {
    return const_iterator(minimum(fake_->left), fake_);
  }

  const_iterator cend() const
  {
    return const_iterator(nullptr, fake_);
  }

private:
  Node* fake_;
  size_t size_;
  Compare comp_;

  void clearSubtree(Node* node)
  {
    if (node == nullptr)
    {
      return;
    }
    clearSubtree(node->left);
    clearSubtree(node->right);
    delete node;
  }

  Node* copy(Node* other, Node* parent)
  {
    if (other == nullptr)
    {
      return nullptr;
    }
    Node* new_node = new Node(other->data.first, other->data.second, parent);
    ++size_;
    new_node->left = copy(other->left, new_node);
    new_node->right = copy(other->right, new_node);
    return new_node;
  }

  Node* minimum(Node* node) const
  {
    if (node == nullptr)
    {
      return nullptr;
    }
    while (node->left != nullptr)
    {
      node = node->left;
    }
    return node;
  }

  Node* findNode(const Key& key) const
  {
    Node* current = fake_->left;
    while (current != nullptr)
    {
      if (comp_(key, current->data.first))
      {
        current = current->left;
      }
      else if (comp_(current->data.first, key))
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
};

template< class Key, class Value >
class BSTConstIterator
{
  template< class K, class V, class C >
  friend class BSTree;

public:
  using Node = typename BSTree< Key, Value >::Node;

  BSTConstIterator()
  : node_(nullptr)
  , fake_(nullptr)
  {}

  const std::pair< Key, Value >& operator*() const
  {
    return node_->data;
  }

  const std::pair< Key, Value >* operator->() const
  {
    return &(node_->data);
  }

  BSTConstIterator& operator++()
  {
    if (node_ == nullptr)
    {
      return *this;
    }

    if (node_->right != nullptr)
    {
      node_ = node_->right;
      while (node_->left != nullptr)
      {
        node_ = node_->left;
      }
      return *this;
    }

    Node* parent = node_->parent;
    while (parent != fake_ && node_ == parent->right)
    {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = parent;

    if (node_ == fake_)
    {
      node_ = nullptr;
    }

    return *this;
  }

  BSTConstIterator operator++(int)
  {
    BSTConstIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const BSTConstIterator& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const BSTConstIterator& other) const
  {
    return node_ != other.node_;
  }

private:
  const Node* node_;
  const Node* fake_;

  BSTConstIterator(const Node* node, const Node* fake)
  : node_(node)
  , fake_(fake)
  {}
};

template< class Key, class Value >
class BSTIterator
{
  template< class K, class V, class C >
  friend class BSTree;

public:
  using Node = typename BSTree< Key, Value >::Node;

  BSTIterator()
  : node_(nullptr)
  , fake_(nullptr)
  {}

  operator BSTConstIterator< Key, Value >() const
  {
    return BSTConstIterator< Key, Value >(node_, fake_);
  }

  std::pair< Key, Value >& operator*() const
  {
    return node_->data;
  }

  std::pair< Key, Value >* operator->() const
  {
    return &(node_->data);
  }

  BSTIterator& operator++()
  {
    if (node_ == nullptr)
    {
      return *this;
    }

    if (node_->right != nullptr)
    {
      node_ = node_->right;
      while (node_->left != nullptr)
      {
        node_ = node_->left;
      }
      return *this;
    }

    Node* parent = node_->parent;
    while (parent != fake_ && node_ == parent->right)
    {
      node_ = parent;
      parent = parent->parent;
    }
    node_ = parent;

    if (node_ == fake_)
    {
      node_ = nullptr;
    }

    return *this;
  }

  BSTIterator operator++(int)
  {
    BSTIterator tmp = *this;
    ++(*this);
    return tmp;
  }

  bool operator==(const BSTIterator& other) const
  {
    return node_ == other.node_;
  }

  bool operator!=(const BSTIterator& other) const
  {
    return node_ != other.node_;
  }

private:
  Node* node_;
  Node* fake_;

  BSTIterator(Node* node, Node* fake)
  : node_(node)
  , fake_(fake)
  {}
};

}

#endif
