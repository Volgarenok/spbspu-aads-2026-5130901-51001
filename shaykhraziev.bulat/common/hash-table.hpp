#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>

namespace shaykhraziev
{
  namespace detail
  {
    const std::size_t DEFAULT_BUCKET_SIZE = 4;
  }

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    struct Entry
    {
      Key key;
      Value value;
      bool occupied;

      Entry():
        key(),
        value(),
        occupied(false)
      {}
    };

    explicit HashTable(std::size_t slots, std::size_t bucketSize = detail::DEFAULT_BUCKET_SIZE):
      buckets_(nullptr),
      slots_(slots),
      bucketSize_(bucketSize),
      size_(0),
      hash_(),
      equal_()
    {
      if (slots_ == 0 || bucketSize_ == 0)
      {
        throw std::invalid_argument("invalid hash table capacity");
      }
      buckets_ = new Entry[capacity()];
    }

    ~HashTable()
    {
      delete[] buckets_;
    }

    HashTable(const HashTable&) = delete;
    HashTable(HashTable&&) = delete;
    HashTable& operator=(const HashTable&) = delete;
    HashTable& operator=(HashTable&&) = delete;

    void clear() noexcept
    {
      for (std::size_t i = 0; i < capacity(); ++i)
      {
        buckets_[i].occupied = false;
      }
      size_ = 0;
    }

    bool empty() const noexcept
    {
      return size_ == 0;
    }

    std::size_t size() const noexcept
    {
      return size_;
    }

    std::size_t slots() const noexcept
    {
      return slots_;
    }

    std::size_t bucketSize() const noexcept
    {
      return bucketSize_;
    }

    std::size_t capacity() const noexcept
    {
      return slots_ * bucketSize_;
    }

  private:
    Entry* buckets_;
    std::size_t slots_;
    std::size_t bucketSize_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;
  };
}

#endif
