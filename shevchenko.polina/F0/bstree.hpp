#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <vector>
#include <stack>

namespace shevchenko
{

enum Color
{
  RED,
  BLACK
};

template< class Key, class Value, class Compare = std::less< Key > >
class BSTree
{
public:
  struct Node
  {
    std::pair< Key, Value > data;
    Node* left;
    Node* right;
    Node* parent;
    Color color;

    Node(const Key& key, const Value& value, Node* par = nullptr)
    : data(key, value)
    , left(nullptr)
    , right(nullptr)
    , parent(par)
    , color(RED)
    {
    }

    Node(const std::pair< Key, Value >& pair, Node* par = nullptr)
    : data(pair)
    , left(nullptr)
    , right(nullptr)
    , parent(par)
    , color(RED)
    {
    }
  };

  BSTree()
  : root_(nullptr)
  , size_(0)
  , comp_(Compare())
  {
  }

  BSTree(const BSTree& other)
  : root_(nullptr)
  , size_(0)
  , comp_(other.comp_)
  {
    root_ = copy(other.root_, nullptr);
  }

  BSTree(BSTree&& other) noexcept
  : root_(other.root_)
  , size_(other.size_)
  , comp_(std::move(other.comp_))
  {
    other.root_ = nullptr;
    other.size_ = 0;
  }

  ~BSTree()
  {
    clear();
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
      root_ = other.root_;
      size_ = other.size_;
      comp_ = std::move(other.comp_);
      other.root_ = nullptr;
      other.size_ = 0;
    }
    return *this;
  }

  void swap(BSTree& other) noexcept
  {
    std::swap(root_, other.root_);
    std::swap(size_, other.size_);
    std::swap(comp_, other.comp_);
  }


  void insert(const Key& key, const Value& value)
  {
    Node* parent = nullptr;
    Node* current = root_;

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

    if (parent == nullptr)
    {
      root_ = new_node;
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

    insertFixup(new_node);
  }

  void insert(const std::pair< Key, Value >& pair)
  {
    insert(pair.first, pair.second);
  }

  void erase(const Key& key)
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      return;
    }
    eraseNode(node);
  }

  bool contains(const Key& key) const
  {
    return findNode(key) != nullptr;
  }

  Value& at(const Key& key)
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return node->data.second;
  }

  const Value& at(const Key& key) const
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      throw std::out_of_range("key not found");
    }
    return node->data.second;
  }

  Value& operator[](const Key& key)
  {
    Node* node = findNode(key);
    if (node == nullptr)
    {
      insert(key, Value());
      node = findNode(key);
    }
    return node->data.second;
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
    clearSubtree(root_);
    root_ = nullptr;
    size_ = 0;
  }

  std::vector< Key > getKeys() const
  {
    std::vector< Key > keys;
    inorderCollect(keys);
    return keys;
  }

private:
  Node* root_;
  size_t size_;
  Compare comp_;

  void inorderCollect(std::vector< Key >& keys) const
  {
    std::stack< Node* > stack;
    Node* current = root_;

    while (current != nullptr || !stack.empty())
    {
      while (current != nullptr)
      {
        stack.push(current);
        current = current->left;
      }
      current = stack.top();
      stack.pop();
      keys.push_back(current->data.first);
      current = current->right;
    }
  }

  void clearSubtree(Node* node)
  {
    if (node == nullptr) return;
    clearSubtree(node->left);
    clearSubtree(node->right);
    delete node;
  }

  Node* copy(Node* other, Node* parent)
  {
    if (other == nullptr) return nullptr;
    Node* new_node = new Node(other->data.first, other->data.second);
    new_node->parent = parent;
    new_node->color = other->color;
    new_node->left = copy(other->left, new_node);
    new_node->right = copy(other->right, new_node);
    ++size_;
    return new_node;
  }

  Node* findNode(const Key& key) const
  {
    Node* current = root_;
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

  void rotateLeft(Node* node)
  {
    Node* right_child = node->right;
    node->right = right_child->left;

    if (right_child->left != nullptr)
    {
      right_child->left->parent = node;
    }

    right_child->parent = node->parent;

    if (node->parent == nullptr)
    {
      root_ = right_child;
    }
    else if (node == node->parent->left)
    {
      node->parent->left = right_child;
    }
    else
    {
      node->parent->right = right_child;
    }

    right_child->left = node;
    node->parent = right_child;
  }

  void rotateRight(Node* node)
  {
    Node* left_child = node->left;
    node->left = left_child->right;

    if (left_child->right != nullptr)
    {
      left_child->right->parent = node;
    }

    left_child->parent = node->parent;

    if (node->parent == nullptr)
    {
      root_ = left_child;
    }
    else if (node == node->parent->left)
    {
      node->parent->left = left_child;
    }
    else
    {
      node->parent->right = left_child;
    }

    left_child->right = node;
    node->parent = left_child;
  }

  void insertFixup(Node* node)
  {
    while (node != root_ && node->parent->color == RED)
    {
      Node* parent = node->parent;
      Node* grandparent = parent->parent;

      if (parent == grandparent->left)
      {
        Node* uncle = grandparent->right;

        if (uncle != nullptr && uncle->color == RED)
        {
          parent->color = BLACK;
          uncle->color = BLACK;
          grandparent->color = RED;
          node = grandparent;
        }
        else
        {
          if (node == parent->right)
          {
            rotateLeft(parent);
            node = parent;
            parent = node->parent;
          }

          parent->color = BLACK;
          grandparent->color = RED;
          rotateRight(grandparent);
        }
      }
      else
      {
        Node* uncle = grandparent->left;

        if (uncle != nullptr && uncle->color == RED)
        {
          parent->color = BLACK;
          uncle->color = BLACK;
          grandparent->color = RED;
          node = grandparent;
        }
        else
        {
          if (node == parent->left)
          {
            rotateRight(parent);
            node = parent;
            parent = node->parent;
          }

          parent->color = BLACK;
          grandparent->color = RED;
          rotateLeft(grandparent);
        }
      }
    }

    root_->color = BLACK;
  }


  Node* minimum(Node* node)
  {
    while (node->left != nullptr)
    {
      node = node->left;
    }
    return node;
  }

  Node* maximum(Node* node)
  {
    while (node->right != nullptr)
    {
      node = node->right;
    }
    return node;
  }

  void transplant(Node* old_node, Node* new_node)
  {
    if (old_node->parent == nullptr)
    {
      root_ = new_node;
    }
    else if (old_node == old_node->parent->left)
    {
      old_node->parent->left = new_node;
    }
    else
    {
      old_node->parent->right = new_node;
    }

    if (new_node != nullptr)
    {
      new_node->parent = old_node->parent;
    }
  }

  void eraseNode(Node* node)
  {
    if (node == nullptr) return;

    Node* to_delete = node;
    Node* replacement = nullptr;
    Node* child = nullptr;
    Color original_color = to_delete->color;

    if (node->left == nullptr)
    {
      child = node->right;
      transplant(node, node->right);
    }
    else if (node->right == nullptr)
    {
      child = node->left;
      transplant(node, node->left);
    }
    else
    {
      to_delete = minimum(node->right);
      original_color = to_delete->color;
      child = to_delete->right;

      if (to_delete->parent == node)
      {
        if (child != nullptr)
        {
          child->parent = to_delete;
        }
      }
      else
      {
        transplant(to_delete, to_delete->right);
        to_delete->right = node->right;
        to_delete->right->parent = to_delete;
      }

      transplant(node, to_delete);
      to_delete->left = node->left;
      to_delete->left->parent = to_delete;
      to_delete->color = node->color;
    }

    delete node;
    --size_;

    if (original_color == BLACK && child != nullptr)
    {
      eraseFixup(child);
    }
    else if (original_color == BLACK && child == nullptr)
    {
    }

    if (root_ != nullptr)
    {
      root_->color = BLACK;
    }
  }

  void eraseFixup(Node* node)
  {
    while (node != root_ && node->color == BLACK)
    {
      Node* parent = node->parent;

      if (node == parent->left)
      {
        Node* brother = parent->right;

        if (brother->color == RED)
        {
          brother->color = BLACK;
          parent->color = RED;
          rotateLeft(parent);
          brother = parent->right;
        }

        if ((brother->left == nullptr || brother->left->color == BLACK) &&
            (brother->right == nullptr || brother->right->color == BLACK))
        {
          brother->color = RED;
          node = parent;
        }
        else
        {
          if (brother->right == nullptr || brother->right->color == BLACK)
          {
            if (brother->left != nullptr)
            {
              brother->left->color = BLACK;
            }
            brother->color = RED;
            rotateRight(brother);
            brother = parent->right;
          }

          brother->color = parent->color;
          parent->color = BLACK;
          if (brother->right != nullptr)
          {
            brother->right->color = BLACK;
          }
          rotateLeft(parent);
          node = root_;
        }
      }
      else
      {
        Node* brother = parent->left;

        if (brother->color == RED)
        {
          brother->color = BLACK;
          parent->color = RED;
          rotateRight(parent);
          brother = parent->left;
        }

        if ((brother->left == nullptr || brother->left->color == BLACK) &&
            (brother->right == nullptr || brother->right->color == BLACK))
        {
          brother->color = RED;
          node = parent;
        }
        else
        {
          if (brother->left == nullptr || brother->left->color == BLACK)
          {
            if (brother->right != nullptr)
            {
              brother->right->color = BLACK;
            }
            brother->color = RED;
            rotateLeft(brother);
            brother = parent->left;
          }

          brother->color = parent->color;
          parent->color = BLACK;
          if (brother->left != nullptr)
          {
            brother->left->color = BLACK;
          }
          rotateRight(parent);
          node = root_;
        }
      }
    }

    if (node != nullptr)
    {
      node->color = BLACK;
    }
  }
};

}
#endif

