#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <limits>
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

    bool add(const Key& key, const Value& value)
    {
      if (findEntry(key))
      {
        throw std::logic_error("duplicate key");
      }

      Entry* entry = findFreeEntry(key);
      if (!entry)
      {
        throw std::overflow_error("hash table is full");
      }
      entry->key = key;
      entry->value = value;
      entry->occupied = true;
      ++size_;
      return true;
    }

    bool set(const Key& key, const Value& value)
    {
      Entry* entry = findEntry(key);
      if (entry)
      {
        entry->value = value;
        return false;
      }
      add(key, value);
      return true;
    }

    bool drop(const Key& key)
    {
      Entry* entry = findEntry(key);
      if (!entry)
      {
        return false;
      }
      entry->occupied = false;
      --size_;
      return true;
    }

    bool has(const Key& key) const
    {
      return findEntry(key) != nullptr;
    }

    Value* find(const Key& key)
    {
      Entry* entry = findEntry(key);
      return entry ? &entry->value : nullptr;
    }

    const Value* find(const Key& key) const
    {
      const Entry* entry = findEntry(key);
      return entry ? &entry->value : nullptr;
    }

    Value& get(const Key& key)
    {
      Entry* entry = findEntry(key);
      if (!entry)
      {
        throw std::out_of_range("missing key");
      }
      return entry->value;
    }

    const Value& get(const Key& key) const
    {
      const Entry* entry = findEntry(key);
      if (!entry)
      {
        throw std::out_of_range("missing key");
      }
      return entry->value;
    }

  private:
    Entry* buckets_;
    std::size_t slots_;
    std::size_t bucketSize_;
    std::size_t size_;
    Hash hash_;
    Equal equal_;

    std::size_t firstSlot(const Key& key) const
    {
      return hash_(key) % slots_;
    }

    std::size_t flatIndex(std::size_t slot, std::size_t index) const
    {
      return slot * bucketSize_ + index;
    }

    Entry* findEntry(const Key& key) noexcept
    {
      std::size_t start = firstSlot(key);
      for (std::size_t offset = 0; offset < slots_; ++offset)
      {
        std::size_t slot = (start + offset) % slots_;
        for (std::size_t i = 0; i < bucketSize_; ++i)
        {
          Entry& entry = buckets_[flatIndex(slot, i)];
          if (entry.occupied && equal_(entry.key, key))
          {
            return &entry;
          }
        }
      }
      return nullptr;
    }

    const Entry* findEntry(const Key& key) const noexcept
    {
      std::size_t start = firstSlot(key);
      for (std::size_t offset = 0; offset < slots_; ++offset)
      {
        std::size_t slot = (start + offset) % slots_;
        for (std::size_t i = 0; i < bucketSize_; ++i)
        {
          const Entry& entry = buckets_[flatIndex(slot, i)];
          if (entry.occupied && equal_(entry.key, key))
          {
            return &entry;
          }
        }
      }
      return nullptr;
    }

    Entry* findFreeEntry(const Key& key) noexcept
    {
      std::size_t start = firstSlot(key);
      for (std::size_t offset = 0; offset < slots_; ++offset)
      {
        std::size_t slot = (start + offset) % slots_;
        for (std::size_t i = 0; i < bucketSize_; ++i)
        {
          Entry& entry = buckets_[flatIndex(slot, i)];
          if (!entry.occupied)
          {
            return &entry;
          }
        }
      }
      return nullptr;
    }
  };
}

#endif
