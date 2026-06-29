#ifndef KARPENKO_MULTIMAP_HPP
#define KARPENKO_MULTIMAP_HPP

#include "avl_tree.hpp"
#include "list.hpp"
#include <utility>

namespace karpenko
{

template < typename Key, typename Value, typename Compare = std::less< Key > >
class Multimap
{
public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair< const Key, Value >;

private:
  using Tree = AVLTree< Key, List< Value >, Compare >;
  using TreeNode = typename Tree::Node;

public:
  class iterator
  {
  public:
    iterator() = default;

    iterator(typename Tree::iterator tree_it, LIter< Value > list_it, Tree* tree_ptr = nullptr)
      : tree_it_(tree_it), list_it_(list_it), tree_ptr_(tree_ptr)
    {}

    value_type operator*() const
    {
      const Key& key = tree_it_.node->data.first;
      return std::make_pair(key, *list_it_);
    }

    iterator& operator++()
    {
      ++list_it_;
      if (list_it_ == tree_it_->second.end())
      {
        ++tree_it_;
        if (tree_ptr_ && tree_it_ != tree_ptr_->end())
        {
          list_it_ = tree_it_->second.begin();
        }
        else
        {
          tree_it_ = typename Tree::iterator();
          list_it_ = LIter< Value >();
        }
      }
      return *this;
    }

    iterator operator++(int)
    {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& other) const
    {
      return tree_it_ == other.tree_it_ && list_it_ == other.list_it_;
    }

    bool operator!=(const iterator& other) const
    {
      return !(*this == other);
    }

  private:
    typename Tree::iterator tree_it_;
    LIter< Value > list_it_;
    Tree* tree_ptr_ = nullptr;
  };

  class const_iterator
  {
  public:
    const_iterator() = default;

    const_iterator(typename Tree::const_iterator tree_it, LCIter< Value > list_it, const Tree* tree_ptr = nullptr)
      : tree_it_(tree_it), list_it_(list_it), tree_ptr_(tree_ptr)
    {
    }

    value_type operator*() const
    {
      const Key& key = tree_it_->first;
      return std::make_pair(key, *list_it_);
    }

    const_iterator& operator++()
    {
      ++list_it_;
      if (list_it_ == tree_it_->second.end())
      {
        ++tree_it_;
        if (tree_ptr_ && tree_it_ != tree_ptr_->end())
        {
          list_it_ = tree_it_->second.begin();
        }
        else
        {
          tree_it_ = typename Tree::const_iterator();
          list_it_ = LCIter< Value >();
        }
      }
      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& other) const
    {
      return tree_it_ == other.tree_it_ && list_it_ == other.list_it_;
    }

    bool operator!=(const const_iterator& other) const
    {
      return !(*this == other);
    }

  private:
    typename Tree::const_iterator tree_it_;
    LCIter< Value > list_it_;
    const Tree* tree_ptr_ = nullptr;
  };

  Multimap() = default;

  void insert(const Key& key, const Value& value)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      List< Value > lst;
      lst.push_back(value);
      tree_.push(key, std::move(lst));
    }
    else
    {
      auto node = const_cast< TreeNode* >(it.node);
      node->data.second.push_back(value);
    }
  }

  void insert(const Key& key, Value&& value)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      List< Value > lst;
      lst.push_back(std::move(value));
      tree_.push(key, std::move(lst));
    }
    else
    {
      auto node = const_cast< TreeNode* >(it.node);
      node->data.second.push_back(std::move(value));
    }
  }

  size_t count(const Key& key) const
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      return 0;
    }
    return it->second.size();
  }

  bool contains(const Key& key) const
  {
    return tree_.find(key) != tree_.end();
  }

  void erase_one(const Key& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      return;
    }
    auto node = const_cast< TreeNode* >(it.node);
    List< Value >& lst = node->data.second;
    if (!lst.empty())
    {
      lst.pop_front();
      if (lst.empty())
      {
        tree_.drop(key);
      }
    }
  }

  void erase_all(const Key& key)
  {
    try
    {
      tree_.drop(key);
    }
    catch (const std::out_of_range&)
    {}
  }

  const List< Value >& at(const Key& key) const
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      throw std::out_of_range("key not found");
    }
    return it->second;
  }

  iterator find(const Key& key)
  {
    auto tree_it = tree_.find(key);
    if (tree_it == tree_.end())
    {
      return end();
    }
    auto node = const_cast< TreeNode* >(tree_it.node);
    return iterator(typename Tree::iterator(node), node->data.second.begin(), &tree_);
  }

  const_iterator find(const Key& key) const
  {
    auto tree_it = tree_.find(key);
    if (tree_it == tree_.end())
    {
      return end();
    }
    return const_iterator(tree_it, tree_it->second.begin(), &tree_);
  }

  iterator begin()
  {
    auto tree_it = tree_.begin();
    if (tree_it == tree_.end())
    {
      return end();
    }
    auto node = const_cast< TreeNode* >(tree_it.node);
    return iterator(typename Tree::iterator(node), node->data.second.begin(), &tree_);
  }

  const_iterator begin() const
  {
    auto tree_it = tree_.begin();
    if (tree_it == tree_.end())
    {
      return end();
    }
    return const_iterator(tree_it, tree_it->second.begin(), &tree_);
  }

  const_iterator cbegin() const
  {
    return begin();
  }

  iterator end()
  {
    return iterator(typename Tree::iterator(), LIter< Value >(), &tree_);
  }

  const_iterator end() const
  {
    return const_iterator(typename Tree::const_iterator(), LCIter< Value >(), &tree_);
  }

  const_iterator cend() const
  {
    return end();
  }

  bool empty() const
  {
    return tree_.begin() == tree_.end();
  }

  size_t size() const
  {
    size_t total = 0;
    for (auto it = begin(); it != end(); ++it)
    {
      ++total;
    }
    return total;
  }

  size_t height() const
  {
    return tree_.height();
  }

  void clear()
  {
    tree_ = Tree();
  }

private:
  Tree tree_;
};

}

#endif
