#ifndef KARPENKO_TREE_NODE_HPP
#define KARPENKO_TREE_NODE_HPP

#include <utility>

namespace karpenko {

template <typename Key, typename Value>
struct TreeNode
{
  std::pair<const Key, Value> data_;
  TreeNode* left_;
  TreeNode* right_;
  TreeNode* parent_;

  TreeNode(const Key& k, Value v)
    : data_(k, std::move(v)),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr)
  {
  }

  TreeNode()
    : data_(Key(), Value()),
      left_(nullptr),
      right_(nullptr),
      parent_(nullptr)
  {
  }
};

}

#endif
