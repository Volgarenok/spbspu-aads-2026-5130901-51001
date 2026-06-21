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

    Value *getOrCreate(const Key &key, bool &inserted)
    {
      Node *parent = nil_;
      Node *current = root_;
      while (current != nil_)
      {
        parent = current;
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
          inserted = false;
          return &current->value_;
        }
      }
      Node *const node = new Node();
      node->key_ = key;
      node->left_ = nil_;
      node->right_ = nil_;
      node->parent_ = parent;
      node->color_ = detail::red;
      if (parent == nil_)
      {
        root_ = node;
      }
      else if (key < parent->key_)
      {
        parent->left_ = node;
      }
      else
      {
        parent->right_ = node;
      }
      insertFixup(node);
      ++size_;
      inserted = true;
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

    void leftRotate(Node *node)
    {
      Node *const pivot = node->right_;
      node->right_ = pivot->left_;
      if (pivot->left_ != nil_)
      {
        pivot->left_->parent_ = node;
      }
      pivot->parent_ = node->parent_;
      if (node->parent_ == nil_)
      {
        root_ = pivot;
      }
      else if (node == node->parent_->left_)
      {
        node->parent_->left_ = pivot;
      }
      else
      {
        node->parent_->right_ = pivot;
      }
      pivot->left_ = node;
      node->parent_ = pivot;
    }

    void rightRotate(Node *node)
    {
      Node *const pivot = node->left_;
      node->left_ = pivot->right_;
      if (pivot->right_ != nil_)
      {
        pivot->right_->parent_ = node;
      }
      pivot->parent_ = node->parent_;
      if (node->parent_ == nil_)
      {
        root_ = pivot;
      }
      else if (node == node->parent_->right_)
      {
        node->parent_->right_ = pivot;
      }
      else
      {
        node->parent_->left_ = pivot;
      }
      pivot->right_ = node;
      node->parent_ = pivot;
    }

    void insertFixup(Node *node)
    {
      while (node->parent_->color_ == detail::red)
      {
        if (node->parent_ == node->parent_->parent_->left_)
        {
          Node *const uncle = node->parent_->parent_->right_;
          if (uncle->color_ == detail::red)
          {
            node->parent_->color_ = detail::black;
            uncle->color_ = detail::black;
            node->parent_->parent_->color_ = detail::red;
            node = node->parent_->parent_;
          }
          else
          {
            if (node == node->parent_->right_)
            {
              node = node->parent_;
              leftRotate(node);
            }
            node->parent_->color_ = detail::black;
            node->parent_->parent_->color_ = detail::red;
            rightRotate(node->parent_->parent_);
          }
        }
        else
        {
          Node *const uncle = node->parent_->parent_->left_;
          if (uncle->color_ == detail::red)
          {
            node->parent_->color_ = detail::black;
            uncle->color_ = detail::black;
            node->parent_->parent_->color_ = detail::red;
            node = node->parent_->parent_;
          }
          else
          {
            if (node == node->parent_->left_)
            {
              node = node->parent_;
              rightRotate(node);
            }
            node->parent_->color_ = detail::black;
            node->parent_->parent_->color_ = detail::red;
            leftRotate(node->parent_->parent_);
          }
        }
      }
      root_->color_ = detail::black;
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
