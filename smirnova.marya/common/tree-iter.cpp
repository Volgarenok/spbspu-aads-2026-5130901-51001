#include "tree-iter.hpp"

namespace smirnova
{

TreeNodeBase* nextNodeBase(TreeNodeBase* node, TreeNodeBase* fake)
{
  if (node == fake)
  {
    return fake;
  }

  if (node->right_ != fake)
  {
    node = node->right_;
    while (node->left_ != fake)
      node = node->left_;
    return node;
  }

  TreeNodeBase* parent = node->parent_;

  while (parent != fake && node == parent->right_)
  {
    node = parent;
    parent = parent->parent_;
  }

  return parent;
}

}

