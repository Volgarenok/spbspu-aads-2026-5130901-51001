#ifndef CUCKOO_HT_HPP
#define CUCKOO_HT_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>
#include <algorithm>
#include <functional>
#include "../common/siphash.hpp"

namespace vishnyakov
{
  template< class Key, class Value, class Hash1 = SipHash, class Hash2 = SipHash, class Equal = std::equal_to< Key > >
  class CuckooHashTable
  {
  public:
    using value_type = std::pair< const Key, Value >;

    struct Node
    {
      value_type data_;
      bool occupied_;

      Node():
        data_(Key(), Value()),
        occupied_(false)
      {
      }

      Node(const Key& key, const Value& value):
        data_(key, value),
        occupied_(true)
      {
      }

      Node(Key&& key, Value&& value):
        data_(std::move(key), std::move(value)),
        occupied_(true)
      {
      }
    };

    CuckooHashTable();
    explicit CuckooHashTable(std::size_t initial_capacity);
    CuckooHashTable(const CuckooHashTable& other);
    CuckooHashTable(CuckooHashTable&& other) noexcept;
    ~CuckooHashTable();

    CuckooHashTable& operator=(const CuckooHashTable& other);
    CuckooHashTable& operator=(CuckooHashTable&& other) noexcept;

    bool empty() const noexcept;
    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    void add(const Key& key, const Value& value);
    void add(Key&& key, Value&& value);
    Value drop(const Key& key);
    bool has(const Key& key) const;
    Value& at(const Key& key);
    const Value& at(const Key& key) const;
    Value& operator[](const Key& key);

    void clear() noexcept;
    void rehash(std::size_t new_capacity);

  private:
    static constexpr std::size_t DEFAULT_CAPACITY = 16;
    static constexpr std::size_t MAX_LOOP = 100;
    static constexpr std::size_t GOLDEN_RATIO = 2654435761U;

    Node* table1_;
    Node* table2_;
    std::size_t table_size_;
    std::size_t size_;
    Hash1 hash1_;
    Hash2 hash2_;
    Equal equal_;

    std::size_t index1(const Key& key) const;
    std::size_t index2(const Key& key) const;
    void rehash_impl();
    Node* find_node(const Key& key) const;
  };

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable():
    table1_(nullptr),
    table2_(nullptr),
    table_size_(DEFAULT_CAPACITY),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    table1_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    table2_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      new (&table1_[i]) Node();
      new (&table2_[i]) Node();
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(std::size_t initial_capacity):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(initial_capacity > 0 ? initial_capacity : DEFAULT_CAPACITY),
    size_(0),
    hash1_(),
    hash2_(),
    equal_()
  {
    table1_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    table2_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      new (&table1_[i]) Node();
      new (&table2_[i]) Node();
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(const CuckooHashTable& other):
    table1_(nullptr),
    table2_(nullptr),
    table_size_(other.table_size_),
    size_(other.size_),
    hash1_(other.hash1_),
    hash2_(other.hash2_),
    equal_(other.equal_)
  {
    table1_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    table2_ = static_cast< Node* >(::operator new(sizeof(Node) * table_size_));
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      new (&table1_[i]) Node(other.table1_[i]);
      new (&table2_[i]) Node(other.table2_[i]);
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::CuckooHashTable(CuckooHashTable&& other) noexcept:
    table1_(other.table1_),
    table2_(other.table2_),
    table_size_(other.table_size_),
    size_(other.size_),
    hash1_(std::move(other.hash1_)),
    hash2_(std::move(other.hash2_)),
    equal_(std::move(other.equal_))
  {
    other.table1_ = nullptr;
    other.table2_ = nullptr;
    other.table_size_ = 0;
    other.size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::~CuckooHashTable()
  {
    clear();
    if (table1_)
    {
      ::operator delete(table1_);
    }
    if (table2_)
    {
      ::operator delete(table2_);
    }
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(const CuckooHashTable& other)
  {
    if (this != &other)
    {
      CuckooHashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  CuckooHashTable< Key, Value, Hash1, Hash2, Equal >& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator=(CuckooHashTable&& other) noexcept
  {
    if (this != &other)
    {
      swap(other);
      other.clear();
    }
    return *this;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::empty() const noexcept
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::size() const noexcept
  {
    return size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::capacity() const noexcept
  {
    return table_size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::add(const Key& key, const Value& value)
  {
    if (has(key))
    {
      return;
    }

    if (size_ >= table_size_)
    {
      rehash_impl();
    }

    Node* current = new Node(key, value);
    for (std::size_t loop = 0; loop < MAX_LOOP; ++loop)
    {
      std::size_t idx1 = index1(current->data_.first);
      if (!table1_[idx1].occupied_)
      {
        table1_[idx1] = *current;
        delete current;
        ++size_;
        return;
      }
      std::swap(current, &table1_[idx1]);

      std::size_t idx2 = index2(current->data_.first);
      if (!table2_[idx2].occupied_)
      {
        table2_[idx2] = *current;
        delete current;
        ++size_;
        return;
      }
      std::swap(current, &table2_[idx2]);
    }

    delete current;
    rehash_impl();
    add(key, value);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::add(Key&& key, Value&& value)
  {
    add(key, value);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash_impl()
  {
    std::size_t new_size = table_size_ * 2;
    CuckooHashTable new_table(new_size);

    for (std::size_t i = 0; i < table_size_; ++i)
    {
      if (table1_[i].occupied_)
      {
        new_table.add(table1_[i].data_.first, table1_[i].data_.second);
      }
      if (table2_[i].occupied_)
      {
        new_table.add(table2_[i].data_.first, table2_[i].data_.second);
      }
    }

    swap(new_table);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::rehash(std::size_t new_capacity)
  {
    if (new_capacity < size_)
    {
      new_capacity = size_ * 2;
    }
    if (new_capacity == 0)
    {
      new_capacity = DEFAULT_CAPACITY;
    }

    CuckooHashTable new_table(new_capacity);
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      if (table1_[i].occupied_)
      {
        new_table.add(table1_[i].data_.first, table1_[i].data_.second);
      }
      if (table2_[i].occupied_)
      {
        new_table.add(table2_[i].data_.first, table2_[i].data_.second);
      }
    }
    swap(new_table);
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  void CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::clear() noexcept
  {
    for (std::size_t i = 0; i < table_size_; ++i)
    {
      table1_[i].~Node();
      table2_[i].~Node();
      new (&table1_[i]) Node();
      new (&table2_[i]) Node();
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::index1(const Key& key) const
  {
    return hash1_(key) % table_size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  std::size_t CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::index2(const Key& key) const
  {
    return (hash2_(key) * GOLDEN_RATIO) % table_size_;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  typename CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::Node* CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::find_node(const Key& key) const
  {
    std::size_t idx1 = index1(key);
    if (table1_[idx1].occupied_ && equal_(table1_[idx1].data_.first, key))
    {
      return &table1_[idx1];
    }

    std::size_t idx2 = index2(key);
    if (table2_[idx2].occupied_ && equal_(table2_[idx2].data_.first, key))
    {
      return &table2_[idx2];
    }

    return nullptr;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  bool CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::has(const Key& key) const
  {
    return find_node(key) != nullptr;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::at(const Key& key)
  {
    Node* node = find_node(key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data_.second;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  const Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::at(const Key& key) const
  {
    Node* node = find_node(key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    return node->data_.second;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value& CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::operator[](const Key& key)
  {
    Node* node = find_node(key);
    if (node)
    {
      return node->data_.second;
    }
    add(key, Value());
    node = find_node(key);
    return node->data_.second;
  }

  template< class Key, class Value, class Hash1, class Hash2, class Equal >
  Value CuckooHashTable< Key, Value, Hash1, Hash2, Equal >::drop(const Key& key)
  {
    Node* node = find_node(key);
    if (!node)
    {
      throw std::out_of_range("Key not found");
    }
    Value result = std::move(node->data_.second);
    node->occupied_ = false;
    node->data_.~value_type();
    new (&node->data_) value_type(Key(), Value());
    --size_;
    return result;
  }
}

#endif

