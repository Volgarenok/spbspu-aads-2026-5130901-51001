#ifndef KARPENKO_MULTISET_HPP
#define KARPENKO_MULTISET_HPP

#include "avl_tree.hpp"

namespace karpenko
{

template < typename Key, typename Compare = std::less< Key > >
class Multiset
{
public:
  using key_type = Key;
  using value_type = Key;
  using iterator = typename AVLTree< Key, size_t, Compare >::iterator;
  using const_iterator = typename AVLTree< Key, size_t, Compare >::const_iterator;

  Multiset() = default;

  void insert(const Key& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      tree_.push(key, 1);
    }
    else
    {
      auto node = const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(it.node);
      ++node->data.second;
    }
  }

  void insert(Key&& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      tree_.push(std::move(key), 1);
    }
    else
    {
      auto node = const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(it.node);
      ++node->data.second;
    }
  }

  size_t count(const Key& key) const
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      return 0;
    }
    return it->second;
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
    auto node = const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(it.node);
    if (node->data.second > 1)
    {
      --node->data.second;
    }
    else
    {
      tree_.drop(key);
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

  iterator find(const Key& key)
  {
    auto cit = tree_.find(key);
    return iterator(const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(cit.node));
  }

  const_iterator find(const Key& key) const
  {
    return tree_.find(key);
  }

  iterator begin()
  {
    return iterator(const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(tree_.begin().node));
  }

  const_iterator begin() const
  {
    return tree_.begin();
  }

  const_iterator cbegin() const
  {
    return tree_.begin();
  }

  iterator end()
  {
    return iterator(const_cast< typename AVLTree< Key, size_t, Compare >::Node* >(tree_.end().node));
  }

  const_iterator end() const
  {
    return tree_.end();
  }

  const_iterator cend() const
  {
    return tree_.end();
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
      total += it->second;
    }
    return total;
  }

  size_t height() const
  {
    return tree_.height();
  }

  void clear()
  {
    tree_ = AVLTree< Key, size_t, Compare >();
  }

private:
  AVLTree< Key, size_t, Compare > tree_;
};

}

#endif
