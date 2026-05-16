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
      buckets_(nullptr),
      bucketsCount_(slots),
      size_(0)
    {
      if (slots == 0) {
        throw std::invalid_argument("zero hash table size");
      }
      buckets_ = new List< Entry >[slots];
    }

    ~HashTable()
    {
      delete[] buckets_;
    }

    size_t size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

  private:
    List< Entry >* buckets_;
    size_t bucketsCount_;
    size_t size_;
  };
}

#endif
