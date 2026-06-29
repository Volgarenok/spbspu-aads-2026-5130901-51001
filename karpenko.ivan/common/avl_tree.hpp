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
    {
    }

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

  };

}
#endif
