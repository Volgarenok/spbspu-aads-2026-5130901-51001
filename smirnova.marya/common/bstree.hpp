#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

#include "treeNode.hpp"
#include "tree-iter.hpp"

namespace smirnova
{
  template< class T >
  struct Less
  {
    bool operator()(const T& left, const T& right) const
    {
      return left < right;
    }
  };

  template< class Key, class Value >
  using Node = TreeNode< Key, Value >;

  template< class NodeT >
  TreeNodeBase* makeFakeLeaf();

  TreeNodeBase* getMin(TreeNodeBase* node, TreeNodeBase* fake);

  template< class Key, class Value, class Compare = Less< Key > >
  class BSTree
  {
  public:
    using NodeT = Node< Key, Value >;

    using key_type = Key;

    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree():
      root_(nullptr),
      fake_leaf_(makeFakeLeaf< NodeT >()),
      size_(0),
      compare_()
    {}

    BSTree(const BSTree& other):
      root_(nullptr),
      fake_leaf_(makeFakeLeaf< NodeT >()),
      size_(other.size_),
      compare_(other.compare_)
    {
      root_ = copy(other.root_, fake_leaf_, other.fake_leaf_);
    }

    BSTree(BSTree&& other) noexcept:
      root_(other.root_),
      fake_leaf_(other.fake_leaf_),
      size_(other.size_),
      compare_(std::move(other.compare_))
    {
      other.root_ = nullptr;
      other.fake_leaf_ = makeFakeLeaf< NodeT >();
      other.size_ = 0;
    }

    BSTree& operator=(BSTree other)
    {
      swap(other);
      return *this;
    }

    ~BSTree()
    {
      clear(root_);
      delete static_cast< NodeT* >(fake_leaf_);
    }

    void swap(BSTree& other) noexcept;

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    iterator begin() noexcept;

    iterator end() noexcept;

    const_iterator begin() const noexcept;

    const_iterator end() const noexcept;

    const_iterator cbegin() const noexcept;

    const_iterator cend() const noexcept;

    bool contains(const Key& key) const noexcept;

    void push(const Key& key, const Value& value);

    Value& get(const Key& key);

    const Value& get(const Key& key) const;

    Value drop(const Key& key);

    size_t height() const;

    size_t height(const_iterator it) const;

    iterator rotateLeft(iterator it);

    iterator rotateRight(iterator it);

    iterator rotateLargeLeft(iterator it);

    iterator rotateLargeRight(iterator it);

  private:
    TreeNodeBase* root_;
    TreeNodeBase* fake_leaf_;
    size_t size_;
    Compare compare_;

    void clear(TreeNodeBase* node) noexcept;

    TreeNodeBase* copy(TreeNodeBase* other,
      TreeNodeBase* parent,
      TreeNodeBase* otherFake
    );

    NodeT* findNode(const Key& key) const noexcept;

    size_t height(TreeNodeBase* node) const noexcept;

    void transplant(TreeNodeBase* u, TreeNodeBase* v);

    TreeNodeBase* rotateLeftNode(TreeNodeBase* x);

    TreeNodeBase* rotateRightNode(TreeNodeBase* x);

    TreeNodeBase* rotateLargeLeftNode(TreeNodeBase* x);

    TreeNodeBase* rotateLargeRightNode(TreeNodeBase* x);
  };
}

#include "bstree_impl.hpp"

#endif

