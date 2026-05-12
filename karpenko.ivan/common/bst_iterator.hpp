#ifndef KARPENKO_BST_ITERATOR_HPP
#define KARPENKO_BST_ITERATOR_HPP

#include "tree_node.hpp"
#include <ostream>

namespace karpenko {

template <typename Key, typename Value>
class BSTIterator
{
public:
  using Node = TreeNode<Key, Value>;

  explicit BSTIterator(Node* n)
    : node_(n)
  {
  }

  std::pair<const Key, Value>& operator*() const
  {
    return node_->data_;
  }

  std::pair<const Key, Value>* operator->() const
  {
    return &node_->data_;
  }

  BSTIterator& operator++()
  {
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      Node* parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
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
    return node_ == other.node_;
  }

  bool operator!=(const BSTIterator& other) const
  {
    return node_ != other.node_;
  }

private:
  Node* node_;
};

template <typename Key, typename Value>
class BSTConstIterator
{
public:
  using Node = TreeNode<Key, Value>;

  explicit BSTConstIterator(const Node* n)
    : node_(n)
  {
  }

  const std::pair<const Key, Value>& operator*() const
  {
    return node_->data_;
  }

  const std::pair<const Key, Value>* operator->() const
  {
    return &node_->data_;
  }

  BSTConstIterator& operator++()
  {
    if (node_->right_)
    {
      node_ = node_->right_;
      while (node_->left_)
      {
        node_ = node_->left_;
      }
    }
    else
    {
      const Node* parent = node_->parent_;
      while (parent && node_ == parent->right_)
      {
        node_ = parent;
        parent = parent->parent_;
      }
      node_ = parent;
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
    return node_ == other.node_;
  }

  bool operator!=(const BSTConstIterator& other) const
  {
    return node_ != other.node_;
  }

private:
  const Node* node_;
};

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const BSTIterator<Key, Value>& it)
{
  if (it.node_)
  {
    os << "iterator( key=" << it.node_->data_.first << " )";
  }
  else
  {
    os << "end-iterator";
  }
  return os;
}

template <typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const BSTConstIterator<Key, Value>& it)
{
  if (it.node_)
  {
    os << "const_iterator( key=" << it.node_->data_.first << " )";
  }
  else
  {
    os << "end-const_iterator";
  }
  return os;
}

}

#endif
