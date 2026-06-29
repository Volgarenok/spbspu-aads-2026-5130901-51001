#ifndef SET_HPP
#define SET_HPP

#include "avl_tree.hpp"
#include <utility>

namespace karpenko
{

template < typename Key, typename Compare = std::less< Key > >
class Set
{
public:
  using key_type = Key;
  using value_type = Key;
  using iterator = typename AVLTree< Key, bool, Compare >::iterator;
  using const_iterator = typename AVLTree< Key, bool, Compare >::const_iterator;

  Set() = default;

  void insert(const Key& key)
  {
    tree_.push(key, true);
  }

  void insert(Key&& key)
  {
    tree_.push(std::move(key), true);
  }

  bool contains(const Key& key) const
  {
    return tree_.find(key) != tree_.end();
  }

  bool erase(const Key& key)
  {
    try
    {
      tree_.drop(key);
      return true;
    }
    catch (const std::out_of_range&)
    {
      return false;
    }
  }

  iterator find(const Key& key)
  {
    return tree_.find(key);
  }

  const_iterator find(const Key& key) const
  {
    return tree_.find(key);
  }

  iterator begin()
  {
    return tree_.begin();
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
    return tree_.end();
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
    size_t sz = 0;
    for (auto it = begin(); it != end(); ++it)
    {
      ++sz;
    }
    return sz;
  }

  size_t height() const
  {
    return tree_.height();
  }

  void clear()
  {
    AVLTree< Key, bool, Compare > empty;
    tree_ = std::move(empty);
  }

private:
  AVLTree< Key, bool, Compare > tree_;
};

}

#endif
