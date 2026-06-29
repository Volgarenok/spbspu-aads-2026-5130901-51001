#ifndef AVL_NODE_HPP
#define AVL_NODE_HPP

#include "tree_node.hpp"

namespace karpenko
{
  template < typename Key, typename Value >
  struct AVLNode :
  public TreeNode< Key, Value >
  {
    int height_;

    AVLNode(const Key &key, Value value)
        : TreeNode<Key, Value>(key, std::move(value)),
          height_(1)
    {}

    AVLNode()
        : TreeNode<Key, Value>(),
          height_(0)
    {}
  };
}

#endif
