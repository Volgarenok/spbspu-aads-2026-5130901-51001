#ifndef KRIVOSHAPOV_RBTREE_HPP
#define KRIVOSHAPOV_RBTREE_HPP

#include <cstddef>

namespace krivoshapov
{
  namespace detail
  {
    enum NodeColor
    {
      red,
      black
    };

    template <typename Key, typename Value>
    struct TreeNode
    {
      Key key_;
      Value value_;
      NodeColor color_;
      TreeNode<Key, Value> *parent_;
      TreeNode<Key, Value> *left_;
      TreeNode<Key, Value> *right_;

      TreeNode() : key_(),
                   value_(),
                   color_(red),
                   parent_(nullptr),
                   left_(nullptr),
                   right_(nullptr)
      {
      }
    };
  }

  template <typename Key, typename Value>
  class RBTree
  {
  public:
    RBTree() : nil_(makeNil()),
               root_(nullptr),
               size_(0)
    {
      root_ = nil_;
    }

    RBTree(const RBTree<Key, Value> &) = delete;

    ~RBTree()
    {
      destroy(root_);
      delete nil_;
    }

    RBTree<Key, Value> &operator=(const RBTree<Key, Value> &) = delete;

    std::size_t getSize() const
    {
      return size_;
    }

    const Value *find(const Key &key) const
    {
      const Node *node = findNode(key);
      if (node == nil_)
      {
        return nullptr;
      }
      return &node->value_;
    }

    Value *find(const Key &key)
    {
      Node *node = findNode(key);
      if (node == nil_)
      {
        return nullptr;
      }
      return &node->value_;
    }

  private:
    using Node = detail::TreeNode<Key, Value>;

    Node *nil_;
    Node *root_;
    std::size_t size_;

    static Node *makeNil()
    {
      Node *const node = new Node();
      node->color_ = detail::black;
      node->parent_ = node;
      node->left_ = node;
      node->right_ = node;
      return node;
    }

    Node *findNode(const Key &key) const
    {
      Node *current = root_;
      while (current != nil_)
      {
        if (key < current->key_)
        {
          current = current->left_;
        }
        else if (current->key_ < key)
        {
          current = current->right_;
        }
        else
        {
          return current;
        }
      }
      return nil_;
    }

    void destroy(Node *node)
    {
      if (node == nil_)
      {
        return;
      }
      destroy(node->left_);
      destroy(node->right_);
      delete node;
    }
  };
}

#endif
