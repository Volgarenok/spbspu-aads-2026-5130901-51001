#ifndef MAP_HPP
#define MAP_HPP

#include "avl_tree.hpp"
#include <utility>
#include <stdexcept>

namespace karpenko
{

template < typename Key, typename Value, typename Compare = std::less< Key > >
class Map
{
public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair< const Key, Value >;
  using iterator = typename AVLTree< Key, Value, Compare >::iterator;
  using const_iterator = typename AVLTree< Key, Value, Compare >::const_iterator;

  Map() = default;

  void insert(const Key& key, const Value& value)
  {
    tree_.push(key, value);
  }

  void insert(const Key& key, Value&& value)
  {
    tree_.push(key, std::move(value));
  }

  bool contains(const Key& key) const
  {
    return tree_.find(key) != tree_.end();
  }

  Value& operator[](const Key& key)
  {
    auto it = tree_.find(key);
    if (it != tree_.end())
    {
      return const_cast< Value& >(it->second);
    }
    tree_.push(key, Value{});
    auto it2 = tree_.find(key);
    return const_cast< Value& >(it2->second);
  }

  Value& at(const Key& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      throw std::out_of_range("key not found");
    }
    return const_cast< Value& >(it->second);
  }

  const Value& at(const Key& key) const
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      throw std::out_of_range("key not found");
    }
    return it->second;
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

  size_t erase(const Key& key)
  {
    return erase(key) ? 1 : 0;
  }

  iterator find(const Key& key)
  {
    auto it = tree_.find(key);
    return iterator(const_cast< typename AVLTree< Key, Value, Compare >::Node* >(it.node));
  }

  const_iterator find(const Key& key) const
  {
    return tree_.find(key);
  }

  iterator begin()
  {
    return iterator(const_cast< typename AVLTree< Key, Value, Compare >::Node* >(tree_.begin().node));
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
    return iterator(const_cast< typename AVLTree< Key, Value, Compare >::Node* >(tree_.end().node));
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
    AVLTree< Key, Value, Compare > empty;
    tree_ = std::move(empty);
  }

private:
  AVLTree< Key, Value, Compare > tree_;
};

}

#endif
