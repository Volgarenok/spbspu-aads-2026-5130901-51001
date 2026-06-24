#ifndef YARMOLINSKAYA_BSTREE_HPP
#define YARMOLINSKAYA_BSTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

namespace yarmolinskaya
{
  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    struct Node
    {
      std::pair< Key, Value > data;
      Node * left;
      Node * right;
      Node * parent;

      Node(const Key & key, const Value & value):
        data(key, value),
        left(nullptr),
        right(nullptr),
        parent(nullptr)
      {}
    };

    class ConstIterator
    {
    public:
      ConstIterator();

      const std::pair< Key, Value > & operator*() const;
      const std::pair< Key, Value > * operator->() const;

      ConstIterator & operator++();

      bool operator==(const ConstIterator & other) const;
      bool operator!=(const ConstIterator & other) const;

    private:
      Node * node_;

      explicit ConstIterator(Node * node);

      friend class BSTree< Key, Value, Compare >;
    };

    BSTree();
    BSTree(const BSTree & other);
    BSTree & operator=(const BSTree & other);
    ~BSTree();

    void insert(const Key & key, const Value & value);
    bool contains(const Key & key) const;
    Value & at(const Key & key);
    const Value & at(const Key & key) const;

    void clear();

    ConstIterator begin() const;
    ConstIterator end() const;

    ConstIterator find(const Key & key) const;

    ConstIterator rotateLeft(ConstIterator it);
    ConstIterator rotateRight(ConstIterator it);

    std::size_t height() const;

  private:
    Node fakeRoot_;
    Compare compare_;

    Node * getRoot() const;
    void setRoot(Node * node);

    Node * copyTree(Node * other, Node * parent);
    void clear(Node * node);
    Node * findNode(const Key & key) const;

    std::size_t height(Node * node) const;

    Node * minimum(Node * node) const;
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::ConstIterator::ConstIterator():
    node_(nullptr)
  {}

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::ConstIterator::ConstIterator(Node * node):
    node_(node)
  {}

  template< class Key, class Value, class Compare >
  const std::pair< Key, Value > &
  BSTree< Key, Value, Compare >::ConstIterator::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value, class Compare >
  const std::pair< Key, Value > *
  BSTree< Key, Value, Compare >::ConstIterator::operator->() const
  {
    return std::addressof(node_->data);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator &
  BSTree< Key, Value, Compare >::ConstIterator::operator++()
  {
    if (node_->right != nullptr) {
      node_ = node_->right;

      while (node_->left != nullptr) {
        node_ = node_->left;
      }
    } else {
      Node * parent = node_->parent;

      while (parent != nullptr && node_ == parent->right) {
        node_ = parent;
        parent = parent->parent;
      }

      node_ = parent;
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::ConstIterator::operator==(
      const ConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::ConstIterator::operator!=(
      const ConstIterator & other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    fakeRoot_(Key(), Value())
  {
    fakeRoot_.left = nullptr;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree & other):
    fakeRoot_(Key(), Value())
  {
    fakeRoot_.left = copyTree(other.getRoot(), &fakeRoot_);
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > &
  BSTree< Key, Value, Compare >::operator=(const BSTree & other)
  {
    if (this != std::addressof(other)) {
      clear();
      fakeRoot_.left = copyTree(other.getRoot(), &fakeRoot_);
    }

    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node *
  BSTree< Key, Value, Compare >::getRoot() const
  {
    return fakeRoot_.left;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::setRoot(Node * node)
  {
    fakeRoot_.left = node;

    if (node != nullptr) {
      node->parent = &fakeRoot_;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::insert(
      const Key & key,
      const Value & value)
  {
    Node * newNode = new Node(key, value);

    if (getRoot() == nullptr) {
      setRoot(newNode);
      return;
    }

    Node * current = getRoot();

    while (true) {
      if (compare_(key, current->data.first)) {
        if (current->left == nullptr) {
          current->left = newNode;
          newNode->parent = current;
          return;
        }

        current = current->left;
      } else if (compare_(current->data.first, key)) {
        if (current->right == nullptr) {
          current->right = newNode;
          newNode->parent = current;
          return;
        }

        current = current->right;
      } else {
        current->data.second = value;
        delete newNode;
        return;
      }
    }
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::contains(const Key & key) const
  {
    return findNode(key) != nullptr;
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::at(const Key & key)
  {
    Node * node = findNode(key);

    if (node == nullptr) {
      throw std::out_of_range("Key not found");
    }

    return node->data.second;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::at(const Key & key) const
  {
    Node * node = findNode(key);

    if (node == nullptr) {
      throw std::out_of_range("Key not found");
    }

    return node->data.second;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    clear(getRoot());
    fakeRoot_.left = nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear(Node * node)
  {
    if (node == nullptr) {
      return;
    }

    clear(node->left);
    clear(node->right);

    delete node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node *
  BSTree< Key, Value, Compare >::copyTree(Node * other, Node * parent)
  {
    if (other == nullptr) {
      return nullptr;
    }

    Node * node = new Node(other->data.first, other->data.second);

    node->parent = parent;
    node->left = copyTree(other->left, node);
    node->right = copyTree(other->right, node);

    return node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator
  BSTree< Key, Value, Compare >::begin() const
  {
    return ConstIterator(minimum(getRoot()));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator
  BSTree< Key, Value, Compare >::end() const
  {
    return ConstIterator(const_cast< Node * >(&fakeRoot_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node *
  BSTree< Key, Value, Compare >::minimum(Node * node) const
  {
    if (node == nullptr) {
      return const_cast< Node * >(&fakeRoot_);
    }

    while (node->left != nullptr) {
      node = node->left;
    }

    return node;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::Node *
  BSTree< Key, Value, Compare >::findNode(const Key & key) const
  {
    Node * current = getRoot();

    while (current != nullptr) {
      if (compare_(key, current->data.first)) {
        current = current->left;
      } else if (compare_(current->data.first, key)) {
        current = current->right;
      } else {
        return current;
      }
    }

    return nullptr;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator
  BSTree< Key, Value, Compare >::find(const Key & key) const
  {
    Node * node = findNode(key);

    if (node == nullptr) {
      return end();
    }

    return ConstIterator(node);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator
  BSTree< Key, Value, Compare >::rotateLeft(ConstIterator it)
  {
    Node * x = it.node_;

    if (x == nullptr || x->right == nullptr) {
      return it;
    }

    Node * y = x->right;

    x->right = y->left;

    if (y->left != nullptr) {
      y->left->parent = x;
    }

    y->parent = x->parent;

    if (x->parent->left == x) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }

    y->left = x;
    x->parent = y;

    return ConstIterator(y);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::ConstIterator
  BSTree< Key, Value, Compare >::rotateRight(ConstIterator it)
  {
    Node * y = it.node_;

    if (y == nullptr || y->left == nullptr) {
      return it;
    }

    Node * x = y->left;

    y->left = x->right;

    if (x->right != nullptr) {
      x->right->parent = y;
    }

    x->parent = y->parent;

    if (y->parent->left == y) {
      y->parent->left = x;
    } else {
      y->parent->right = x;
    }

    x->right = y;
    y->parent = x;

    return ConstIterator(x);
  }

  template< class Key, class Value, class Compare >
  std::size_t BSTree< Key, Value, Compare >::height() const
  {
    return height(getRoot());
  }

  template< class Key, class Value, class Compare >
  std::size_t BSTree< Key, Value, Compare >::height(Node * node) const
  {
    if (node == nullptr) {
      return 0;
    }

    std::size_t leftHeight = height(node->left);
    std::size_t rightHeight = height(node->right);

    return 1 + ((leftHeight > rightHeight) ? leftHeight : rightHeight);
  }
}

#endif
