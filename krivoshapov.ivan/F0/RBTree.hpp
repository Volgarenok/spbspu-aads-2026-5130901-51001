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
    class ConstIterator
    {
    public:
      ConstIterator(const detail::TreeNode<Key, Value> *node,
                    const detail::TreeNode<Key, Value> *sentinel) : node_(node),
                                                                    sentinel_(sentinel)
      {
      }

      bool operator==(const ConstIterator &other) const
      {
        return node_ == other.node_;
      }

      bool operator!=(const ConstIterator &other) const
      {
        return node_ != other.node_;
      }

      ConstIterator &operator++()
      {
        node_ = successor(node_, sentinel_);
        return *this;
      }

      const Key &getKey() const
      {
        return node_->key_;
      }

      const Value &getValue() const
      {
        return node_->value_;
      }

    private:
      const detail::TreeNode<Key, Value> *node_;
      const detail::TreeNode<Key, Value> *sentinel_;
    };

    RBTree() : nil_(makeNil()),
               root_(nullptr),
               size_(0)
    {
      root_ = nil_;
    }

    RBTree(const RBTree<Key, Value> &) = delete;

    RBTree(RBTree<Key, Value> &&other) noexcept : nil_(other.nil_),
                                                  root_(other.root_),
                                                  size_(other.size_)
    {
      other.nil_ = makeNil();
      other.root_ = other.nil_;
      other.size_ = 0;
    }

    ~RBTree()
    {
      destroy(root_);
      delete nil_;
    }

    RBTree<Key, Value> &operator=(const RBTree<Key, Value> &) = delete;

    RBTree<Key, Value> &operator=(RBTree<Key, Value> &&other) noexcept
    {
      if (this != &other)
      {
        destroy(root_);
        delete nil_;
        nil_ = other.nil_;
        root_ = other.root_;
        size_ = other.size_;
        other.nil_ = makeNil();
        other.root_ = other.nil_;
        other.size_ = 0;
      }
      return *this;
    }

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

    bool erase(const Key &key)
    {
      Node *const node = findNode(key);
      if (node == nil_)
      {
        return false;
      }
      removeNode(node);
      --size_;
      return true;
    }

    void clear()
    {
      destroy(root_);
      root_ = nil_;
      size_ = 0;
    }

    ConstIterator begin() const
    {
      return ConstIterator(minimum(root_), nil_);
    }

    ConstIterator end() const
    {
      return ConstIterator(nil_, nil_);
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

    Node *minimum(Node *node) const
    {
      if (node == nil_)
      {
        return nil_;
      }
      while (node->left_ != nil_)
      {
        node = node->left_;
      }
      return node;
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

    void transplant(Node *target, Node *replacement)
    {
      if (target->parent_ == nil_)
      {
        root_ = replacement;
      }
      else if (target == target->parent_->left_)
      {
        target->parent_->left_ = replacement;
      }
      else
      {
        target->parent_->right_ = replacement;
      }
      replacement->parent_ = target->parent_;
    }

    void removeNode(Node *node)
    {
      Node *moved = node;
      detail::NodeColor removedColor = moved->color_;
      Node *fixupNode = nil_;
      if (node->left_ == nil_)
      {
        fixupNode = node->right_;
        transplant(node, node->right_);
      }
      else if (node->right_ == nil_)
      {
        fixupNode = node->left_;
        transplant(node, node->left_);
      }
      else
      {
        moved = minimum(node->right_);
        removedColor = moved->color_;
        fixupNode = moved->right_;
        if (moved->parent_ == node)
        {
          fixupNode->parent_ = moved;
        }
        else
        {
          transplant(moved, moved->right_);
          moved->right_ = node->right_;
          moved->right_->parent_ = moved;
        }
        transplant(node, moved);
        moved->left_ = node->left_;
        moved->left_->parent_ = moved;
        moved->color_ = node->color_;
      }
      delete node;
      if (removedColor == detail::black)
      {
        eraseFixup(fixupNode);
      }
      nil_->parent_ = nil_;
    }

    void eraseFixup(Node *node)
    {
      while (node != root_ && node->color_ == detail::black)
      {
        if (node == node->parent_->left_)
        {
          Node *sibling = node->parent_->right_;
          if (sibling->color_ == detail::red)
          {
            sibling->color_ = detail::black;
            node->parent_->color_ = detail::red;
            leftRotate(node->parent_);
            sibling = node->parent_->right_;
          }
          if (sibling->left_->color_ == detail::black && sibling->right_->color_ == detail::black)
          {
            sibling->color_ = detail::red;
            node = node->parent_;
          }
          else
          {
            if (sibling->right_->color_ == detail::black)
            {
              sibling->left_->color_ = detail::black;
              sibling->color_ = detail::red;
              rightRotate(sibling);
              sibling = node->parent_->right_;
            }
            sibling->color_ = node->parent_->color_;
            node->parent_->color_ = detail::black;
            sibling->right_->color_ = detail::black;
            leftRotate(node->parent_);
            node = root_;
          }
        }
        else
        {
          Node *sibling = node->parent_->left_;
          if (sibling->color_ == detail::red)
          {
            sibling->color_ = detail::black;
            node->parent_->color_ = detail::red;
            rightRotate(node->parent_);
            sibling = node->parent_->left_;
          }
          if (sibling->right_->color_ == detail::black && sibling->left_->color_ == detail::black)
          {
            sibling->color_ = detail::red;
            node = node->parent_;
          }
          else
          {
            if (sibling->left_->color_ == detail::black)
            {
              sibling->right_->color_ = detail::black;
              sibling->color_ = detail::red;
              leftRotate(sibling);
              sibling = node->parent_->left_;
            }
            sibling->color_ = node->parent_->color_;
            node->parent_->color_ = detail::black;
            sibling->left_->color_ = detail::black;
            rightRotate(node->parent_);
            node = root_;
          }
        }
      }
      node->color_ = detail::black;
    }

    static const Node *subtreeMinimum(const Node *node, const Node *sentinel)
    {
      if (node == sentinel)
      {
        return sentinel;
      }
      while (node->left_ != sentinel)
      {
        node = node->left_;
      }
      return node;
    }

    static const Node *successor(const Node *node, const Node *sentinel)
    {
      if (node == sentinel)
      {
        return sentinel;
      }
      if (node->right_ != sentinel)
      {
        return subtreeMinimum(node->right_, sentinel);
      }
      const Node *parent = node->parent_;
      while (parent != sentinel && node == parent->right_)
      {
        node = parent;
        parent = parent->parent_;
      }
      return parent;
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
