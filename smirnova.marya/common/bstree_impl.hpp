#ifndef BSTREE_IMPL_HPP
#define BSTREE_IMPL_HPP

#include "bstree.hpp"
#include <cstddef>

namespace smirnova
{
  template< class NodeT >
  TreeNodeBase* makeFakeLeaf()
  {
    NodeT* tmp = new NodeT();

    tmp->parent_ = tmp;
    tmp->left_ = tmp;
    tmp->right_ = tmp;

    return tmp;
  }

  inline TreeNodeBase* getMin(TreeNodeBase* node, TreeNodeBase* fake)
  {
    while (node->left_ != fake)
    {
      node = node->left_;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::swap(BSTree& other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(fake_leaf_, other.fake_leaf_);
    std::swap(size_, other.size_);
    std::swap(compare_, other.compare_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::begin() noexcept
  {
    if (!root_)
    {
      return end();
    }

    return iterator(getMin(root_, fake_leaf_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::end() noexcept
  {
    return iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::begin() const noexcept
  {
    return cbegin();
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::end() const noexcept
  {
    return cend();
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cbegin() const noexcept
  {
    if (!root_)
    {
      return cend();
    }

    return const_iterator(
      getMin(root_, fake_leaf_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTConstIterator< Key, Value > BSTree< Key, Value, Compare >::cend() const noexcept
  {
    return const_iterator(fake_leaf_, fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(const Key& key) const noexcept
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree<Key, Value, Compare>::push(const Key& key, const Value& value)
  {
    using NodeT = Node< Key, Value >;

    if (!root_)
    {
      NodeT* tmp = new NodeT(key, value, fake_leaf_);

      tmp->left_ = fake_leaf_;
      tmp->right_ = fake_leaf_;

      root_ = tmp;

      ++size_;
      return;
    }

    TreeNodeBase* node = root_;
    TreeNodeBase* parent = fake_leaf_;

    while (node != fake_leaf_)
    {
      parent = node;

      NodeT* n = static_cast< NodeT* >(node);

      if (compare_(key, n->data_.first))
      {
        node = node->left_;
      }
      else if (compare_(n->data_.first, key))
      {
        node = node->right_;
      }
      else
      {
        n->data_.second = value;
        return;
      }
    }

    NodeT* tmp = new NodeT(key, value, parent);

    tmp->left_ = fake_leaf_;
    tmp->right_ = fake_leaf_;

    NodeT* p = static_cast< NodeT* >(parent);

    if (compare_(key, p->data_.first))
    {
      parent->left_ = tmp;
    }
    else
    {
      parent->right_ = tmp;
    }

    ++size_;
  }

  template< class Key, class Value, class Compare >
  Value& BSTree< Key, Value, Compare >::get(const Key& key)
  {
    using NodeT = Node< Key, Value >;

    NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  const Value& BSTree< Key, Value, Compare >::get(const Key& key) const
  {
    using NodeT = Node< Key, Value >;

    NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    return node->data_.second;
  }

  template< class Key, class Value, class Compare >
  Value BSTree< Key, Value, Compare >::drop(const Key& key)
  {
    using NodeT = Node< Key, Value >;

    NodeT* node = findNode(key);

    if (!node)
    {
      throw std::runtime_error("key not found");
    }

    Value result = node->data_.second;

    if (node->left_ == fake_leaf_)
    {
      transplant(node, node->right_);
    }
    else if (node->right_ == fake_leaf_)
    {
      transplant(node, node->left_);
    }
    else
    {
      NodeT* next = static_cast< NodeT* >(getMin(node->right_, fake_leaf_));

      if (next->parent_ != node)
      {
        transplant(next, next->right_);

        next->right_ = node->right_;
        next->right_->parent_ = next;
      }

      transplant(node, next);

      next->left_ = node->left_;
      next->left_->parent_ = next;
    }

    delete node;

    --size_;

    if (size_ == 0)
    {
      root_ = nullptr;
    }

    return result;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return height(root_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return height(
      const_cast< TreeNodeBase* >(it.current_)
    );
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLeft(iterator it)
  {
    return iterator(rotateLeftNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateRight(iterator it)
  {
    return iterator(rotateRightNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeLeft(iterator it)
  {
    return iterator(rotateLargeLeftNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  BSTIterator< Key, Value > BSTree< Key, Value, Compare >::rotateLargeRight(iterator it)
  {
    return iterator(rotateLargeRightNode(it.current_), fake_leaf_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(TreeNodeBase* node) noexcept
  {
    using NodeT = Node< Key, Value >;

    if (!node || node == fake_leaf_)
    {
      return;
    }

    clear(node->left_);
    clear(node->right_);

    delete static_cast< NodeT* >(node);
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase* BSTree< Key, Value, Compare >::copy(
    TreeNodeBase* other,
    TreeNodeBase* parent,
    TreeNodeBase* otherFake
  )
  {
    using NodeT = Node< Key, Value >;

    if (!other || other == otherFake)
    {
      return fake_leaf_;
    }

    NodeT* otherNode = static_cast< NodeT* >(other);

    NodeT* tmp = new NodeT(otherNode->data_.first, otherNode->data_.second, parent);

    try
    {
      tmp->left_ = copy(other->left_, tmp, otherFake);
      tmp->right_ = copy(other->right_, tmp, otherFake);
    }
    catch (...)
    {
      delete tmp;
      throw;
    }

    return tmp;
  }

  template< class Key, class Value, class Compare >
  Node< Key, Value >* BSTree<Key, Value, Compare>::findNode(const Key& key) const noexcept
  {
    using NodeT = Node< Key, Value >;

    TreeNodeBase* node = root_;

    while (node && node != fake_leaf_)
    {
      NodeT* n = static_cast< NodeT* >(node);

      if (compare_(key, n->data_.first))
      {
        node = node->left_;
      }
      else if (compare_(n->data_.first, key))
      {
        node = node->right_;
      }
      else
      {
        return n;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(TreeNodeBase* node) const noexcept
  {
    if (!node || node == fake_leaf_)
    {
      return 0;
    }

    size_t left = height(node->left_);
    size_t right = height(node->right_);

    return (left > right ? left : right) + 1;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::transplant(TreeNodeBase* u, TreeNodeBase* v)
  {
    if (u->parent_ == fake_leaf_)
    {
      root_ = (v == fake_leaf_) ? nullptr : v;
    }
    else if (u == u->parent_->left_)
    {
      u->parent_->left_ = v;
    }
    else
    {
      u->parent_->right_ = v;
    }

    if (v != fake_leaf_)
    {
      v->parent_ = u->parent_;
    }
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase* BSTree< Key, Value, Compare >::rotateLeftNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
      return x;

    TreeNodeBase* y = x->right_;
    if (!y || y == fake_leaf_)
      return x;

    transplant(x, y);

    x->right_ = y->left_;
    if (y->left_ != fake_leaf_)
      y->left_->parent_ = x;

    y->left_ = x;
    x->parent_ = y;

    return y;
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase* BSTree< Key, Value, Compare >::rotateRightNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
      return x;

    TreeNodeBase* y = x->left_;
    if (!y || y == fake_leaf_)
      return x;

    transplant(x, y);

    x->left_ = y->right_;
    if (y->right_ != fake_leaf_)
      y->right_->parent_ = x;

    y->right_ = x;
    x->parent_ = y;

    return y;
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase* BSTree< Key, Value, Compare >::rotateLargeLeftNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
    {
      return x;
    }
    if (x->right_ == fake_leaf_)
    {
      return x;
    }
    rotateRightNode(x->right_);
    return rotateLeftNode(x);
  }

  template< class Key, class Value, class Compare >
  TreeNodeBase* BSTree< Key, Value, Compare >::rotateLargeRightNode(TreeNodeBase* x)
  {
    if (!x || x == fake_leaf_)
    {
      return x;
    }
    if (x->left_ == fake_leaf_)
    {
      return x;
    }
    rotateLeftNode(x->left_);
    return rotateRightNode(x);
  }

}

#endif

