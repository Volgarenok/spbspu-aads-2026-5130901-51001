#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>

#include "../common/list.hpp"

namespace muraviev
{
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    struct Entry
    {
      Key key;
      Value value;
    };

    explicit HashTable(size_t slots):
      buckets_(new List< Entry >[slots]),
      bucketsCount_(slots),
      size_(0),
      hash_(),
      equal_()
    {
      if (slots == 0) {
        throw std::invalid_argument("zero hash table size");
      }
    }

    ~HashTable()
    {
      delete[] buckets_;
    }

    void add(const Key& key, const Value& value)
    {
      const size_t index = hash_(key) % bucketsCount_;
      for (typename List< Entry >::iter it = buckets_[index].begin();
          it != buckets_[index].end(); ++it) {
        if (equal_(it->key, key)) {
          it->value = value;
          return;
        }
      }
      Entry entry = {key, value};
      if (buckets_[index].empty()) {
        buckets_[index].pushFront(entry);
      } else {
        buckets_[index].insert(buckets_[index].last(), entry);
      }
      ++size_;
    }

    bool has(const Key& key) const
    {
      const size_t index = hash_(key) % bucketsCount_;
      for (typename List< Entry >::c_iter it = buckets_[index].begin();
          it != buckets_[index].end(); ++it) {
        if (equal_(it->key, key)) {
          return true;
        }
      }
      return false;
    }

    Value& at(const Key& key)
    {
      const size_t index = hash_(key) % bucketsCount_;
      for (typename List< Entry >::iter it = buckets_[index].begin();
          it != buckets_[index].end(); ++it) {
        if (equal_(it->key, key)) {
          return it->value;
        }
      }
      throw std::out_of_range("key not found");
    }

  private:
    List< Entry >* buckets_;
    size_t bucketsCount_;
    size_t size_;
    Hash hash_;
    Equal equal_;
  };
}

#endif
