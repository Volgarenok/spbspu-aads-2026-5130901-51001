#ifndef KARPENKO_AVL_TREE_HPP
#define KARPENKO_AVL_TREE_HPP

#include "avl_node.hpp"
#include "bst_iterator.hpp"
#include <functional>
#include <stdexcept>
#include <utility>

namespace karpenko
{

  template <typename Key, typename Value, typename Compare = std::less<Key>>
  class AVLTree
  {
  public:
    using Node = AVLNode<Key, Value>;
    using iterator = BSTIterator<Key, Value>;
    using const_iterator = BSTConstIterator<Key, Value>;

    AVLTree()
        : fake_root_(new Node()),
          cmp_()
    {}

    ~AVLTree()
    {
      clear(fake_root_->left);
      delete fake_root_;
    }

    AVLTree(const AVLTree &) = delete;
    AVLTree &operator=(const AVLTree &) = delete;

    AVLTree(AVLTree &&other) noexcept
        : fake_root_(other.fake_root_),
          cmp_(std::move(other.cmp_))
    {
      other.fake_root_ = new Node();
    }

    AVLTree &operator=(AVLTree &&other) noexcept
    {
      if (this != &other)
      {
        clear(fake_root_->left);
        delete fake_root_;
        fake_root_ = other.fake_root_;
        cmp_ = std::move(other.cmp_);
        other.fake_root_ = new Node();
      }
      return *this;
    }

    void push(const Key &key, Value value)
    {
      if (fake_root_->left == nullptr)
      {
        fake_root_->left = new Node(key, std::move(value));
        fake_root_->left->parent = fake_root_;
        return;
      }
      fake_root_->left = insert(fake_root_->left, key, std::move(value));
      fake_root_->left->parent = fake_root_;
    }

    Value get(const Key &key) const
    {
      Node *node = find_node(key);
      if (!node)
      {
        throw std::out_of_range("key not found");
      }
      return node->data.second;
    }

    Value drop(const Key &key)
    {
      Node *node = find_node(key);
      if (!node)
      {
        throw std::out_of_range("key not found");
      }
      Value result = std::move(node->data.second);
      fake_root_->left = erase(fake_root_->left, key);
      if (fake_root_->left)
      {
        fake_root_->left->parent = fake_root_;
      }
      return result;
    }
    const_iterator find(const Key& key) const
  {
    Node* node = find_node(key);
    return const_iterator(node ? node : fake_root_);
  }

  const_iterator begin() const
  {
    Node* cur = fake_root_->left;
    if (!cur)
    {
      return end();
    }
    while (cur->left)
    {
      cur = static_cast< Node* >(cur->left);
    }
    return const_iterator(cur);
  }

  const_iterator end() const
  {
    return const_iterator(fake_root_);
  }

  size_t height() const
  {
    return node_height(fake_root_->left);
  }

  size_t height(const_iterator it) const
  {
    return node_height(static_cast< const Node* >(it.node));
  }

  const_iterator rotateLeft(const_iterator it)
  {
    Node* x = const_cast< Node* >(static_cast< const Node* >(it.node));
    if (!x || x == fake_root_ || !x->parent || x->parent == fake_root_)
    {
      throw std::logic_error("impossible left rotation");
    }
    Node* p = static_cast< Node* >(x->parent);
    if (x != p->right)
    {
      throw std::logic_error("node is not a right child");
    }

    p->right = x->left;
    if (x->left)
    {
      x->left->parent = p;
    }
    x->left = p;
    x->parent = p->parent;
    if (p->parent->left == p)
    {
      p->parent->left = x;
    }
    else
    {
      p->parent->right = x;
    }
    p->parent = x;

    update_height(p);
    update_height(x);
    return const_iterator(x);
  }

  const_iterator rotateRight(const_iterator it)
  {
    Node* x = const_cast< Node* >(static_cast< const Node* >(it.node));
    if (!x || x == fake_root_ || !x->parent || x->parent == fake_root_)
    {
      throw std::logic_error("impossible right rotation");
    }
    Node* p = static_cast< Node* >(x->parent);
    if (x != p->left)
    {
      throw std::logic_error("node is not a left child");
    }

    p->left = x->right;
    if (x->right)
    {
      x->right->parent = p;
    }
    x->right = p;
    x->parent = p->parent;
    if (p->parent->left == p)
    {
      p->parent->left = x;
    }
    else
    {
      p->parent->right = x;
    }
    p->parent = x;

    update_height(p);
    update_height(x);
    return const_iterator(x);
  }

  const_iterator rotateLargeLeft(const_iterator it)
  {
    Node* x = const_cast< Node* >(static_cast< const Node* >(it.node));
    if (!x || x == fake_root_ || !x->parent || x->parent == fake_root_)
    {
      throw std::logic_error("impossible large left rotation");
    }
    Node* p = static_cast< Node* >(x->parent);
    Node* g = static_cast< Node* >(p->parent);
    if (p != g->right || x != p->left)
    {
      throw std::logic_error("wrong structure for large left");
    }

    rotateRight(const_iterator(x));
    return rotateLeft(const_iterator(x));
  }

  const_iterator rotateLargeRight(const_iterator it)
  {
    Node* x = const_cast< Node* >(static_cast< const Node* >(it.node));
    if (!x || x == fake_root_ || !x->parent || x->parent == fake_root_)
    {
      throw std::logic_error("impossible large right rotation");
    }
    Node* p = static_cast< Node* >(x->parent);
    Node* g = static_cast< Node* >(p->parent);
    if (p != g->left || x != p->right)
    {
      throw std::logic_error("wrong structure for large right");
    }

    rotateLeft(const_iterator(x));
    return rotateRight(const_iterator(x));
  }

private:
  Node* fake_root_;
  Compare cmp_;

  Node* find_node(const Key& key) const
  {
    Node* cur = static_cast< Node* >(fake_root_->left);
    while (cur)
    {
      if (cmp_(key, cur->data.first))
      {
        cur = static_cast< Node* >(cur->left);
      }
      else if (cmp_(cur->data.first, key))
      {
        cur = static_cast< Node* >(cur->right);
      }
      else
      {
        return cur;
      }
    }
    return nullptr;
  }

  int node_height(const Node* node) const
  {
    return node ? node->height_ : 0;
  }

  void update_height(Node* node)
  {
    if (node)
    {
      int left_h = node_height(static_cast< Node* >(node->left));
      int right_h = node_height(static_cast< Node* >(node->right));
      node->height_ = 1 + (left_h > right_h ? left_h : right_h);
    }
  }

  int balance_factor(const Node* node) const
  {
    if (!node)
    {
      return 0;
    }
    return node_height(static_cast< Node* >(node->left)) - node_height(static_cast< Node* >(node->right));
  }

  };

}
  
#endif
