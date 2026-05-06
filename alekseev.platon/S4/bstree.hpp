#ifndef ALEKSEEV_S4_BSTREE_HPP
#define ALEKSEEV_S4_BSTREE_HPP

#include <cstddef>
#include <functional>
#include <utility>

namespace alekseev
{
  namespace detail
  {
    struct BaseNode
    {
      BaseNode* parent;
      BaseNode* left;
      BaseNode* right;
    };

    template< class Key, class Value >
    struct TreeNode: BaseNode
    {
      std::pair< Key, Value > data;
    };
  }

  template< class Key, class Value >
  class BSTIterator;

  template< class Key, class Value >
  class BSTConstIterator;

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using iterator = BSTIterator< Key, Value >;
    using const_iterator = BSTConstIterator< Key, Value >;

    BSTree();
    explicit BSTree(Compare cmp);

    bool empty() const noexcept;
    size_t size() const noexcept;

  private:
    detail::BaseNode fakeRoot_;
    size_t size_;
    Compare cmp_;
  };
}

#include "bstree.tpp"

#endif
