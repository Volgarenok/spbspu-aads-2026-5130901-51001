#ifndef ALEKSEEV_S4_BSTREE_TPP
#define ALEKSEEV_S4_BSTREE_TPP

namespace alekseev
{
  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree():
    fakeRoot_{nullptr, nullptr, nullptr},
    size_(0),
    cmp_(Compare())
  {
  }

  template< class Key, class Value, class Compare >
  BSTree< Key, Value, Compare >::BSTree(Compare cmp):
    fakeRoot_{nullptr, nullptr, nullptr},
    size_(0),
    cmp_(cmp)
  {
  }

  template< class Key, class Value, class Compare >
  bool BSTree< Key, Value, Compare >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Compare >
  size_t BSTree< Key, Value, Compare >::size() const noexcept
  {
    return size_;
  }
}

#endif
