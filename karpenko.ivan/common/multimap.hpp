#ifndef MULTIMAP_HPP
#define MULTIMAP_HPP

#include "avl_tree.hpp"
#include "list.hpp"
#include <utility>
#include <stdexcept>

namespace karpenko
{

template < typename Key, typename Value, typename Compare = std::less< Key > >
class Multimap
{
public:
  using key_type = Key;
  using mapped_type = Value;
  using value_type = std::pair< const Key, Value >;

  Multimap() = default;

  void insert(const Key& key, const Value& value)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      List< Value > lst;
      lst.push_back(value);
      tree_.push(key, lst);
    }
    else
    {
      const_cast< List< Value >& >(it->second).push_back(value);
    }
  }

  void insert(const Key& key, Value&& value)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      List< Value > lst;
      lst.push_back(std::move(value));
      tree_.push(key, lst);
    }
    else
    {
      const_cast< List< Value >& >(it->second).push_back(std::move(value));
    }
  }

  bool contains(const Key& key) const
  {
    return tree_.find(key) != tree_.end();
  }

  size_t count(const Key& key) const
  {
    auto it = tree_.find(key);
    return (it == tree_.end()) ? 0 : it->second.size();
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

  bool erase_one(const Key& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      return false;
    }
    List< Value >& lst = const_cast< List< Value >& >(it->second);
    if (lst.empty())
    {
      return false;
    }
    lst.pop_front();
    if (lst.empty())
    {
      tree_.drop(key);
    }
    return true;
  }

  size_t erase_all(const Key& key)
  {
    auto it = tree_.find(key);
    if (it == tree_.end())
    {
      return 0;
    }
    size_t old_size = it->second.size();
    tree_.drop(key);
    return old_size;
  }

  bool empty() const
  {
    return tree_.begin() == tree_.end();
  }

  size_t size() const
  {
    size_t total = 0;
    for (auto it = tree_.begin(); it != tree_.end(); ++it)
    {
      total += it->second.size();
    }
    return total;
  }

  size_t height() const
  {
    return tree_.height();
  }

  void clear()
  {
    AVLTree< Key, List< Value >, Compare > empty;
    tree_ = std::move(empty);
  }

private:
  AVLTree< Key, List< Value >, Compare > tree_;
};

}

#endif
