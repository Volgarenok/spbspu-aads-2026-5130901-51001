#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

#include "../common/list.hpp"

namespace shevchenko
{

template< class Key, class Value >
struct HashPair
{
  Key first;
  Value second;

  HashPair()
  : first()
  , second()
  {
  }

  HashPair(const Key& key, const Value& value)
  : first(key)
  , second(value)
  {
  }
};

template<
class Key,
class Value,
class Hash,
class Equal = std::equal_to< Key >
>
class HashTable
{
public:
  using value_type = HashPair<Key, Value>;
  using bucket_type = List<value_type>;

  class const_iterator;

  class iterator
  {
    friend class HashTable;
    friend class const_iterator;

  public:
    iterator()
    : table_(nullptr)
    , bucket_index_(0)
    , bucket_iterator_()
    {
    }

    value_type& operator*()
    {
      return *bucket_iterator_;
    }

    value_type* operator->()
    {
      return &(*bucket_iterator_);
    }

    iterator& operator++()
    {
      ++bucket_iterator_;

      if (table_ == nullptr || bucket_index_ >= table_->bucket_count_)
      {
        return *this;
      }

      while (bucket_iterator_ == table_->buckets_[bucket_index_].end())
      {
        ++bucket_index_;

        if (bucket_index_ >= table_->bucket_count_)
        {
          bucket_iterator_ = typename bucket_type::iterator();
          return *this;
        }

        bucket_iterator_ = table_->buckets_[bucket_index_].begin();
      }

      return *this;
    }

    iterator operator++(int)
    {
      iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const iterator& other) const
    {
      return table_ == other.table_ &&
      bucket_index_ == other.bucket_index_ &&
      bucket_iterator_ == other.bucket_iterator_;
    }

    bool operator!=(const iterator& other) const
    {
      return !(*this == other);
    }

  private:
    iterator(HashTable* table, size_t bucket_index, typename bucket_type::iterator bucket_iterator)
    : table_(table)
    , bucket_index_(bucket_index)
    , bucket_iterator_(bucket_iterator)
    {
    }

    HashTable* table_;
    size_t bucket_index_;
    typename bucket_type::iterator bucket_iterator_;
  };

  class const_iterator
  {
    friend class HashTable;

  public:
    const_iterator()
    : table_(nullptr)
    , bucket_index_(0)
    , bucket_iterator_()
    {
    }

    const_iterator(const iterator& other)
    : table_(other.table_)
    , bucket_index_(other.bucket_index_)
    , bucket_iterator_(other.bucket_iterator_)
    {
    }

    const value_type& operator*() const
    {
      return *bucket_iterator_;
    }

    const value_type* operator->() const
    {
      return &(*bucket_iterator_);
    }

    const_iterator& operator++()
    {
      ++bucket_iterator_;

      if (table_ == nullptr || bucket_index_ >= table_->bucket_count_)
      {
        return *this;
      }

      while (bucket_iterator_ == table_->buckets_[bucket_index_].end())
      {
        ++bucket_index_;

        if (bucket_index_ >= table_->bucket_count_)
        {
          bucket_iterator_ = typename bucket_type::const_iterator();
          return *this;
        }

        bucket_iterator_ = table_->buckets_[bucket_index_].cbegin();
      }

      return *this;
    }

    const_iterator operator++(int)
    {
      const_iterator tmp(*this);
      ++(*this);
      return tmp;
    }

    bool operator==(const const_iterator& other) const
    {
      return table_ == other.table_ &&
      bucket_index_ == other.bucket_index_ &&
      bucket_iterator_ == other.bucket_iterator_;
    }

    bool operator!=(const const_iterator& other) const
    {
      return !(*this == other);
    }

  private:
    const_iterator(const HashTable* table, size_t bucket_index, typename bucket_type::const_iterator bucket_iterator)
    : table_(table)
    , bucket_index_(bucket_index)
    , bucket_iterator_(bucket_iterator)
    {
    }

    const HashTable* table_;
    size_t bucket_index_;
    typename bucket_type::const_iterator bucket_iterator_;
  };

  explicit HashTable(size_t bucket_count = 64, size_t bucket_size = 32)
  : bucket_count_(bucket_count)
  , bucket_size_(bucket_size)
  , size_(0)
  , buckets_(new bucket_type[bucket_count])
  , hash_()
  , equal_()
  {
  }

  HashTable(const HashTable& other)
  : bucket_count_(other.bucket_count_)
  , bucket_size_(other.bucket_size_)
  , size_(other.size_)
  , buckets_(new bucket_type[other.bucket_count_])
  , hash_(other.hash_)
  , equal_(other.equal_)
  {
    for (size_t i = 0; i < bucket_count_; ++i)
    {
      for (auto it = other.buckets_[i].cbegin();
           it != other.buckets_[i].cend();
           ++it)
      {
        buckets_[i].pushBack(*it);
      }
    }
  }

  HashTable(HashTable&& other) noexcept
  : bucket_count_(other.bucket_count_)
  , bucket_size_(other.bucket_size_)
  , size_(other.size_)
  , buckets_(other.buckets_)
  , hash_(std::move(other.hash_))
  , equal_(std::move(other.equal_))
  {
    other.bucket_count_ = 0;
    other.bucket_size_ = 0;
    other.size_ = 0;
    other.buckets_ = nullptr;
  }

  ~HashTable()
  {
    delete[] buckets_;
  }

  HashTable& operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }
    return *this;
  }

  HashTable& operator=(HashTable&& other) noexcept
  {
    if (this != &other)
    {
      delete[] buckets_;

      bucket_count_ = other.bucket_count_;
      bucket_size_ = other.bucket_size_;
      size_ = other.size_;
      buckets_ = other.buckets_;
      hash_ = std::move(other.hash_);
      equal_ = std::move(other.equal_);

      other.bucket_count_ = 0;
      other.bucket_size_ = 0;
      other.size_ = 0;
      other.buckets_ = nullptr;
    }
    return *this;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  size_t size() const
  {
    return size_;
  }

  size_t bucket_count() const
  {
    return bucket_count_;
  }

  size_t bucket_size() const
  {
    return bucket_size_;
  }

  void clear()
  {
    delete[] buckets_;
    buckets_ = new bucket_type[bucket_count_];
    size_ = 0;
  }

  bool contains(const Key& key) const
  {
    return find(key) != cend();
  }

  void insert(const Key& key, const Value& value)
  {
    size_t index = getIndex(key);
    bucket_type& bucket = buckets_[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        (*it).second = value;
        return;
      }
    }

    if (bucket.size() >= bucket_size_)
    {
      throw std::overflow_error("bucket overflow");
    }

    bucket.pushBack(value_type(key, value));
    ++size_;
  }

  void erase(const Key& key)
  {
    size_t index = getIndex(key);
    bucket_type& bucket = buckets_[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        bucket.erase(it);
        --size_;
        return;
      }
    }

    throw std::out_of_range("key not found");
  }

  Value& at(const Key& key)
  {
    iterator it = find(key);
    if (it == end())
    {
      throw std::out_of_range("key not found");
    }
    return (*it).second;
  }

  const Value& at(const Key& key) const
  {
    const_iterator it = find(key);
    if (it == cend())
    {
      throw std::out_of_range("key not found");
    }
    return (*it).second;
  }

  iterator find(const Key& key)
  {
    size_t index = getIndex(key);
    bucket_type& bucket = buckets_[index];

    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        return iterator(this, index, it);
      }
    }

    return end();
  }

  const_iterator find(const Key& key) const
  {
    size_t index = getIndex(key);
    const bucket_type& bucket = buckets_[index];

    for (auto it = bucket.cbegin(); it != bucket.cend(); ++it)
    {
      if (equal_((*it).first, key))
      {
        return const_iterator(this, index, it);
      }
    }

    return cend();
  }

  iterator begin()
  {
    for (size_t i = 0; i < bucket_count_; ++i)
    {
      if (!buckets_[i].empty())
      {
        return iterator(this, i, buckets_[i].begin());
      }
    }
    return end();
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator cbegin() const
  {
    for (size_t i = 0; i < bucket_count_; ++i)
    {
      if (!buckets_[i].empty())
      {
        return const_iterator(this, i, buckets_[i].cbegin());
      }
    }
    return cend();
  }

  iterator end()
  {
    return iterator(this, bucket_count_, typename bucket_type::iterator());
  }

  const_iterator end() const
  {
    return cend();
  }

  const_iterator cend() const
  {
    return const_iterator(this, bucket_count_, typename bucket_type::const_iterator());
  }

  void rehash(size_t new_bucket_count)
  {
    if (new_bucket_count == 0)
    {
      throw std::invalid_argument("invalid bucket count");
    }

    HashTable tmp(new_bucket_count, bucket_size_);
    for (const_iterator it = cbegin(); it != cend(); ++it)
    {
      tmp.insert((*it).first, (*it).second);
    }
    swap(tmp);
  }

  void swap(HashTable& other) noexcept
  {
    std::swap(bucket_count_, other.bucket_count_);
    std::swap(bucket_size_, other.bucket_size_);
    std::swap(size_, other.size_);
    std::swap(buckets_, other.buckets_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

private:
  size_t getIndex(const Key& key) const
  {
    if (bucket_count_ == 0)
    {
      throw std::logic_error("empty hash table");
    }
    return hash_(key) % bucket_count_;
  }

  size_t bucket_count_;
  size_t bucket_size_;
  size_t size_;
  bucket_type* buckets_;
  Hash hash_;
  Equal equal_;
};

}

#endif
