#ifndef BSTREE_HPP
#define BSTREE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace vishnyakov
{
  template< class Key, class Value, class Compare >
  class BSTree;

  template< class Key, class Value, class Compare >
  class BSTIter
  {
    friend class BSTree< Key, Value, Compare >;

  public:
    BSTIter();
    BSTIter(const BSTIter&) = default;
    BSTIter& operator=(const BSTIter&) = default;
    ~BSTIter() = default;

    std::pair< const Key, Value >& operator*();
    std::pair< const Key, Value >* operator->();

    BSTIter& operator++();
    BSTIter operator++(int);

    bool operator==(const BSTIter& other) const;
    bool operator!=(const BSTIter& other) const;

  private:
    struct Impl;
    Impl* impl_;
  };

  template< class Key, class Value, class Compare >
  class BSTCIter
  {
    friend class BSTree< Key, Value, Compare >;

  public:
    BSTCIter();
    BSTCIter(const BSTCIter&) = default;
    BSTCIter(const BSTIter< Key, Value, Compare >& other);
    ~BSTCIter() = default;

    BSTCIter& operator=(const BSTCIter&) = default;

    const std::pair< const Key, Value >& operator*() const;
    const std::pair< const Key, Value >* operator->() const;

    BSTCIter& operator++();
    BSTCIter operator++(int);

    bool operator==(const BSTCIter& other) const;
    bool operator!=(const BSTCIter& other) const;

  private:
    struct Impl;
    Impl* impl_;
  };

  template< class Key, class Value, class Compare = std::less< Key > >
  class BSTree
  {
  public:
    using value_type = std::pair< const Key, Value >;
    using iterator = BSTIter< Key, Value, Compare >;
    using const_iterator = BSTCIter< Key, Value, Compare >;

    BSTree();
    BSTree(const BSTree& other);
    BSTree(BSTree&& other) noexcept;
    ~BSTree();

    BSTree& operator=(const BSTree& other);
    BSTree& operator=(BSTree&& other) noexcept;

    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    void push(const Key& key, const Value& value);
    void push(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    iterator rotate_left(iterator pos);
    iterator rotate_right(iterator pos);
    iterator rotate_left_large(iterator pos);
    iterator rotate_right_large(iterator pos);

    std::size_t height() const;
    std::size_t height(const_iterator pos) const;

    void clear();
    void swap(BSTree& other) noexcept;

    Compare key_comp() const;

  private:
    struct Node
    {
      value_type data_;
      Node* left_;
      Node* right_;
      Node* parent_;

      Node(const Key& key, const Value& value, Node* parent = nullptr);
      Node(Key&& key, Value&& value, Node* parent = nullptr);
    };

    Node* root_;
    std::size_t size_;
    Compare comp_;

    Node* copy_tree(Node* node, Node* parent);
    void delete_tree(Node* node);
    Node* find_node(const Key& key) const;
    Node* insert_node(Node* node, const Key& key, const Value& value, Node* parent);
    Node* insert_node(Node* node, Key&& key, Value&& value, Node* parent);
    Node* erase_node(Node* node, const Key& key);
    Node* min_node(Node* node) const;
    Node* max_node(Node* node) const;
    std::size_t get_height(Node* node) const;
    Node* rotate_left_impl(Node* node);
    Node* rotate_right_impl(Node* node);
    Node* rotate_left_large_impl(Node* node);
    Node* rotate_right_large_impl(Node* node);
    iterator make_iter(Node* node) const;
    const_iterator make_citer(Node* node) const;
  };

  template< class Key, class Value, class Compare >
  void swap(BSTree< Key, Value, Compare >& lhs, BSTree< Key, Value, Compare >& rhs) noexcept;
}

#endif

