#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <utility>
#include <functional>
#include <cstddef>
#include <stdexcept>

namespace hachaturyanov
{
  template< class Key, class Value > struct BSTNode {
    std::pair< const Key, Value > data;
    BSTNode* left;
    BSTNode* right;
    BSTNode* parent;
  };

  template< class Key, class Value, class Compare > class BSTree;
  template< class Key, class Value > class BSTIterator;

  template< class Key, class Value > class BSTConstIterator {
    using Node = BSTNode< Key, Value >;
    template< class K, class V, class C > friend class BSTree;

    Node* node_;
    Node* fake_;

    BSTConstIterator(Node* node, Node* fake);

   public:
    using value_type = std::pair< const Key, Value >;

    const value_type & operator*() const;
    const value_type* operator->() const;
    BSTConstIterator & operator++();

    bool operator==(const BSTConstIterator &other) const;
    bool operator!=(const BSTConstIterator &other) const;

    BSTConstIterator(const BSTIterator< Key, Value > &other);
  };

  template< class Key, class Value > class BSTIterator {
    using Node = BSTNode< Key, Value >;
    template< class K, class V, class C > friend class BSTree;
    friend class BSTConstIterator< Key, Value >;

    Node* node_;
    Node* fake_;

    BSTIterator(Node* node, Node* fake);

   public:
    using value_type = std::pair< const Key, Value >;

    value_type & operator*() const;
    value_type* operator->() const;
    BSTIterator & operator++();

    bool operator==(const BSTIterator &other) const;
    bool operator!=(const BSTIterator &other) const;
  };

  template< class Key, class Value, class Compare > class BSTree {
    using Node = BSTNode< Key, Value >;

    Node* root_;
    Node* fake_;

    Node* makeNode_(const Key &key, const Value &value);
    Node* makeFake_();
    Node* findNode_(const Key &key) const;
    size_t height_(Node* n) const;
    void clear_(Node* n);

   public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    BSTree(const BSTree &other);
    ~BSTree();

    BSTree & operator=(const BSTree &other);

    iterator begin();
    iterator end();
    const_iterator begin() const;
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    bool isEmpty() const;
    bool has(const Key &key) const;

    void push(const Key &key, const Value &value);
    Value & get(const Key &key);
    const Value & get(const Key &key) const;
    void drop(const Key &key);

    const_iterator rotateLeft(const_iterator it);
    const_iterator rotateRight(const_iterator it);

    const_iterator rotateLargeLeft(const_iterator it);
    const_iterator rotateLargeRight(const_iterator it);

    size_t height(const_iterator it) const;
    size_t height() const;

    void clear();
  };

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare > & BSTree< Key, Value, Compare >::operator=(const BSTree &other)
  {
    if (this != &other) {
      clear();
      for (auto it = other.cbegin(); it != other.cend(); ++it) {
        push(it->first, it->second);
      }
    }
    return *this;
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(const BSTree &other):
   root_(nullptr),
   fake_(makeFake_())
  {
    for (auto it = other.cbegin(); it != other.cend(); ++it) {
      push(it->first, it->second);
    }
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::has(const Key &key) const
  {
    return findNode_(key);
  }

  template< class Key, class Value, class Compare >
  BSTNode< Key, Value >* BSTree< Key, Value, Compare >::findNode_(const Key &key) const
  {
    if (root_ == nullptr) {
      return nullptr;
    }
    Node* cur = root_;
    Compare cmp;
    while (true) {
      if (cmp(key, cur->data.first)) {
        if (cur->left != fake_) {
          cur = cur->left;
        } else {
          return nullptr;
        }
      } else if (cmp(cur->data.first, key)) {
        if (cur->right != fake_) {
          cur = cur->right;
        } else {
          return nullptr;
        }
      } else {
        return cur;
      }
    }
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(const BSTIterator< Key, Value > &other):
   node_(other.node_),
   fake_(other.fake_)
  {}

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear()
  {
    clear_(root_);
    root_ = nullptr;
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::clear_(Node* n)
  {
    if (n == nullptr || n == fake_) {
      return;
    }
    clear_(n->left);
    clear_(n->right);
    delete n;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height(const_iterator it) const
  {
    return height_(it.node_);
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height() const
  {
    return height_(root_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeRight(const_iterator it)
  {
    Node* X = it.node_;
    rotateLeft(const_iterator(X, fake_));
    return rotateRight(const_iterator(X, fake_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLargeLeft(const_iterator it)
  {
    Node* X = it.node_;
    rotateRight(const_iterator(X, fake_));
    return rotateLeft(const_iterator(X, fake_));
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateRight(const_iterator it)
  {
    Node* X = it.node_;
    Node* Y = X->parent;
    Node* B = X->right;

    Y->left = B;
    if (B != fake_) {
      B->parent = Y;
    }

    X->parent = Y->parent;
    if (Y->parent == fake_) {
      root_ = X;
    } else if (Y == Y->parent->right) {
      Y->parent->right = X;
    } else {
      Y->parent->left = X;
    }

    X->right = Y;
    Y->parent = X;

    return const_iterator(X, fake_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator BSTree< Key, Value, Compare >::rotateLeft(const_iterator it)
  {
    Node* X = it.node_;
    Node* Y = X->parent;
    Node* B = X->left;

    Y->right = B;
    if (B != fake_) {
      B->parent = Y;
    }

    X->parent = Y->parent;
    if (Y->parent == fake_) {
      root_ = X;
    } else if (Y == Y->parent->left) {
      Y->parent->left = X;
    } else {
      Y->parent->right = X;
    }

    X->left = Y;
    Y->parent = X;

    return const_iterator(X, fake_);
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::drop(const Key &key)
  {
    if (root_ == nullptr) {
      throw std::logic_error("Key not found");
    }

    Node* cur = root_;
    Compare cmp;
    while (cur != fake_) {
      if (cmp(key, cur->data.first)) {
        cur = cur->left;
      } else if (cmp(cur->data.first, key)) {
        cur = cur->right;
      } else {
        break;
      }
    }
    if (cur == fake_) {
      throw std::logic_error("Key not found");
    }
    if (cur->left != fake_ && cur->right != fake_) {
      Node* successor = cur->right;
      while (successor->left != fake_) {
        successor = successor->left;
      }
      Node* succChild = successor->right;
      if (successor->parent != cur) {
        successor->parent->left = succChild;
        if (succChild != fake_) {
          succChild->parent = successor->parent;
        }
        successor->right = cur->right;
        cur->right->parent = successor;
      }

      successor->left = cur->left;
      cur->left->parent = successor;
      successor->parent = cur->parent;

      if (cur == root_) {
        root_ = successor;
      } else {
        if (cur->parent->left == cur) {
          cur->parent->left = successor;
        } else {
          cur->parent->right = successor;
        }
      }

      delete cur;
      return;
    }

    Node* child = nullptr;
    if (cur->left != fake_) {
      child = cur->left;
    } else {
      child = cur->right;
    }

    if (cur == root_) {
      if (child == fake_) {
        root_ = nullptr;
      } else {
        root_ = child;
      }
      if (root_ != nullptr) {
        root_->parent = fake_;
      }
    } else {
      Node* parent = cur->parent;
      if (parent->left == cur) {
        parent->left = child;
      } else {
        parent->right = child;
      }
      if (child != fake_) {
        child->parent = parent;
      }
    }

    delete cur;
  }

  template< class Key, class Value, class Compare >
  const Value & BSTree< Key, Value, Compare >::get(const Key &key) const
  {
    return const_cast< BSTree* >(this)->get(key);
  }

  template< class Key, class Value, class Compare >
  Value & BSTree< Key, Value, Compare >::get(const Key &key)
  {
    Node* result = findNode_(key);
    if (!result) {
      throw std::logic_error("Key not found");
    } else {
      return result->data.second;
    }
  }

  template< class Key, class Value, class Compare >
  void BSTree< Key, Value, Compare >::push(const Key &key, const Value &value)
  {
    Node* newNode = makeNode_(key, value);
    if (root_ == nullptr) {
      root_ = newNode;
      root_->parent = fake_;
      return;
    }
    Node* cur = root_;
    Compare cmp;
    bool added = false;
    while (!added) {
      if (cmp(key, cur->data.first)) {
        if (cur->left == fake_) {
          cur->left = newNode;
          newNode->parent = cur;
          added = true;
        } else {
          cur = cur->left;
        }
      } else if (cmp(cur->data.first, key)) {
        if (cur->right == fake_) {
          cur->right = newNode;
          newNode->parent = cur;
          added = true;
        } else {
          cur = cur->right;
        }
      } else {
        delete newNode;
        cur->data.second = value;
        added = true;
      }
    }
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::isEmpty() const
  {
    return root_ == nullptr;
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::cend() const
  {
    return end();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::cbegin() const
  {
    return begin();
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::end() const
  {
    return const_iterator(fake_, fake_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::const_iterator
      BSTree< Key, Value, Compare >::begin() const
  {
    if (root_ == nullptr) {
      return end();
    }
    Node* cur = root_;
    while (cur->left != fake_) {
      cur = cur->left;
    }
    return const_iterator(cur, fake_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::end()
  {
    return iterator(fake_, fake_);
  }

  template< class Key, class Value, class Compare >
  typename BSTree< Key, Value, Compare >::iterator BSTree< Key, Value, Compare >::begin()
  {
    if (root_ == nullptr) {
      return end();
    }
    Node* cur = root_;
    while (cur->left != fake_) {
      cur = cur->left;
    }
    return iterator(cur, fake_);
  }

  template< class Key, class Value, class Compare > BSTree< Key, Value, Compare >::~BSTree()
  {
    clear();
    ::operator delete(fake_);
  }

  template< class Key, class Value, class Compare > BSTree< Key, Value, Compare >::BSTree():
   root_(nullptr),
   fake_(makeFake_())
  {}

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::height_(Node* n) const
  {
    if (n == fake_ || n == nullptr) {
      return 0;
    }
    return 1 + std::max(height_(n->left), height_(n->right));
  }

  template< class Key, class Value, class Compare >
  BSTNode< Key, Value >* BSTree< Key, Value, Compare >::makeFake_()
  {
    Node* fake = static_cast< Node* >(::operator new(sizeof(Node)));
    fake->left = fake;
    fake->right = fake;
    fake->parent = fake;
    return fake;
  }

  template< class Key, class Value, class Compare >
  BSTNode< Key, Value >* BSTree< Key, Value, Compare >::makeNode_(const Key &key, const Value &value)
  {
    return new Node{ { key, value }, fake_, fake_, nullptr };
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator!=(const BSTIterator &other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTIterator< Key, Value >::operator==(const BSTIterator &other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value > & BSTIterator< Key, Value >::operator++()
  {
    if (node_->right != fake_) {
      node_ = node_->right;
      while (node_->left != fake_) {
        node_ = node_->left;
      }
    } else {
      Node* par = node_->parent;
      while (par != fake_ && node_ == par->right) {
        node_ = par;
        par = par->parent;
      }
      node_ = par;
    }
    return *this;
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::value_type* BSTIterator< Key, Value >::operator->() const
  {
    return &node_->data;
  }

  template< class Key, class Value >
  typename BSTIterator< Key, Value >::value_type & BSTIterator< Key, Value >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value >
  BSTIterator< Key, Value >::BSTIterator(Node* node, Node* fake):
   node_(node),
   fake_(fake)
  {}

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator!=(const BSTConstIterator & other) const
  {
    return node_ != other.node_;
  }

  template< class Key, class Value >
  bool BSTConstIterator< Key, Value >::operator==(const BSTConstIterator & other) const
  {
    return node_ == other.node_;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value > & BSTConstIterator< Key, Value >::operator++()
  {
    if (node_->right != fake_) {
      node_ = node_->right;
      while (node_->left != fake_) {
        node_ = node_ ->left;
      }
    } else {
      Node* par = node_->parent;
      while (par != fake_ && node_ == par->right) {
        node_ = par;
        par = par->parent;
      }
      node_ = par;
    }
    return *this;
  }

  template< class Key, class Value >
  const typename BSTConstIterator< Key, Value >::value_type*
      BSTConstIterator< Key, Value >::operator->() const
  {
    return &node_->data;
  }

  template< class Key, class Value >
  const typename BSTConstIterator< Key, Value >::value_type &
      BSTConstIterator< Key, Value >::operator*() const
  {
    return node_->data;
  }

  template< class Key, class Value >
  BSTConstIterator< Key, Value >::BSTConstIterator(Node* node, Node* fake):
   node_(node),
   fake_(fake)
  {}
}

#endif
