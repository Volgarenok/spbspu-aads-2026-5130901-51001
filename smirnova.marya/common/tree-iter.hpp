#ifndef TREEITERATORS_HPP
#define TREEITERATORS_HPP

#include <stdexcept>
#include <utility>

#include "treeNode.hpp"

namespace smirnova
{

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare >
  class BSTree;

  TreeNodeBase* nextNodeBase(TreeNodeBase* node, TreeNodeBase* fake);

  template< class Key, class Value >
  class BSTIterator
  {
  public:
    using NodeT = TreeNode< Key, Value >;

    BSTIterator():
      current_(nullptr),
      fake_(nullptr)
    {}

    BSTIterator(TreeNodeBase* current, TreeNodeBase* fake):
      current_(current),
      fake_(fake)
    {}

    std::pair<Key, Value>& operator*() const
    {
      if (current_ == fake_)
      {
        throw std::runtime_error("bad iterator");
      }

      return static_cast< NodeT* >(current_)->data_;
    }

    std::pair< Key, Value >* operator->() const
    {
      return &static_cast< NodeT* >(current_)->data_;
    }

    BSTIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ = nextNodeBase(current_, fake_);
      }

      return *this;
    }

    BSTIterator operator++(int)
    {
      BSTIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTIterator& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const BSTIterator& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    TreeNodeBase* current_;
    TreeNodeBase* fake_;

    template< class K, class V >
    friend class BSTConstIterator;

    template< class K, class V, class C >
    friend class BSTree;
  };

  template< class Key, class Value >
  class BSTConstIterator
  {
  public:
    using NodeT = TreeNode< Key, Value >;

    BSTConstIterator():
      current_(nullptr),
      fake_(nullptr)
    {}

    BSTConstIterator(const TreeNodeBase* current, const TreeNodeBase* fake):
      current_(current),
      fake_(fake)
    {}

    BSTConstIterator(const BSTIterator<Key, Value>& other):
      current_(other.current_),
      fake_(other.fake_)
    {}

    const std::pair< Key, Value >& operator*() const
    {
      if (current_ == fake_)
      {
        throw std::runtime_error("bad iterator");
      }

      return static_cast< const NodeT* >(current_)->data_;
    }

    const std::pair< Key, Value >* operator->() const
    {
      return &static_cast< const NodeT* >(current_)->data_;
    }

    BSTConstIterator& operator++()
    {
      if (current_ != fake_)
      {
        current_ = nextNodeBase(const_cast< TreeNodeBase* >(current_), const_cast< TreeNodeBase* >(fake_)
        );
      }

      return *this;
    }

    BSTConstIterator operator++(int)
    {
      BSTConstIterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const BSTConstIterator& other) const noexcept
    {
      return current_ == other.current_;
    }

    bool operator!=(const BSTConstIterator& other) const noexcept
    {
      return !(*this == other);
    }

  private:
    const TreeNodeBase* current_;
    const TreeNodeBase* fake_;

    template< class K, class V, class C >
    friend class BSTree;
  };
}

#endif

