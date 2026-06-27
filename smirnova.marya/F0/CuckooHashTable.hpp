#ifndef CUCKOO_HASH_TABLE_HPP
#define CUCKOO_HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include "../common/vector.hpp"
#include "CuckooHashFunc.hpp"

namespace smirnova
{

  template< class Key, class Value, class Hash = DefaultHasher< Key > >
  class CuckooHashTable
  {
  public:
    struct Node
    {
      Key key{};
      Value value{};
      bool used = false;
      bool deleted = false;
    };

    class Iterator
    {
    public:
      using Table = CuckooHashTable;

      Iterator(Table* t = nullptr, size_t i = 0):
        table_(t),
        index_(i)
      {
        skip();
      }

      Node& operator*()
      {
        auto p = resolve(index_);
        return table_->bucket(p.first)[p.second];
      }

      Node* operator->()
      {
        return &(**this);
      }

      Iterator& operator++()
      {
        ++index_;
        skip();
        return *this;
      }

      bool operator!=(const Iterator& other) const
      {
        return table_ != other.table_ || index_ != other.index_;
      }

    private:
      Table* table_;
      size_t index_;

      std::pair< int,size_t > resolve(size_t idx) const
      {
        size_t n = table_->capacity();

        if (idx < n)
        {
          return std::make_pair(0, idx);
        }
        else
        {
          return std::make_pair(1, idx - n);
        }
      }

      void skip()
      {
        if (!table_)
        {
          return;
        }
        size_t total = table_->capacity() * 2;
        while (index_ < total)
        {
          auto p = resolve(index_);
          auto& b = table_->bucket(p.first);
          if (b[p.second].used && !b[p.second].deleted)
          {
            return;
          }
          ++index_;
        }
      }
    };

  private:
    Vector< Node > table1_;
    Vector< Node > table2_;
    size_t capacity_ = 16;
    Hash hash_;

  public:
      CuckooHashTable()
      {
        allocate(capacity_);
      }

      size_t capacity() const
      {
        return capacity_;
      }

      Vector< Node >& bucket(int i)
      {
        if (i == 0)
        {
          return table1_;
        }
        else
        {
          return table2_;
        }
      }

      const Vector< Node >& bucket(int i) const
      {
        return (i == 0) ? table1_ : table2_;
      }

      Value* get(const Key& key)
      {
        size_t i1 = hash_.h1(key) % capacity_;

        if (table1_[i1].used && !table1_[i1].deleted && table1_[i1].key == key)
        {
          return &table1_[i1].value;
        }

        size_t i2 = hash_.h2(key) % capacity_;

        if (table2_[i2].used && !table2_[i2].deleted && table2_[i2].key == key)
        {
          return &table2_[i2].value;
        }
        return nullptr;
      }

      const Value* get(const Key& key) const
      {
        return const_cast< CuckooHashTable* >(this)->get(key);
      }

      bool contains(const Key& key) const
      {
        return get(key) != nullptr;
      }

      double loadFactor() const
      {
        size_t used = 0;

        for (size_t i = 0; i < capacity_; ++i)
        {
          if (table1_[i].used)
          {
            ++used;
          }
          if (table2_[i].used)
          {
            ++used;
          }
        }
        return static_cast< double >(used) / (capacity_ * 2);
      }

      void rehash(size_t newCap)
      {
          Vector< Node > old1 = table1_;
          Vector< Node > old2 = table2_;

          capacity_ = newCap;

          allocate(capacity_);

          for (auto& n : old1)
          {
            if (n.used && !n.deleted)
            {
              insert(n.key, n.value);
            }
          }

          for (auto& n : old2)
          {
            if (n.used && !n.deleted)
            {
              insert(n.key, n.value);
            }
          }
      }

      bool insert(const Key& key, const Value& value)
      {
        if (loadFactor() > 0.7)
        {
          rehash(capacity_ * 2);
        }

        size_t i1 = hash_.h1(key) % capacity_;

        if (!table1_[i1].used)
        {
          table1_[i1] = {key, value, true, false};
          return true;
        }

        size_t i2 = hash_.h2(key) % capacity_;

        if (!table2_[i2].used)
        {
          table2_[i2] = {key, value, true, false};
          return true;
        }
        return false;
      }

      bool erase(const Key& key)
      {
        size_t i1 = hash_.h1(key) % capacity_;

        if (table1_[i1].used && table1_[i1].key == key)
        {
          table1_[i1].deleted = true;
          return true;
        }

        size_t i2 = hash_.h2(key) % capacity_;

        if (table2_[i2].used && table2_[i2].key == key)
        {
          table2_[i2].deleted = true;
          return true;
        }
        return false;
      }

      Iterator begin()
      {
        return Iterator(this, 0);
      }

      Iterator end()
      {
        return Iterator(this, capacity_ * 2);
      }

  private:
    void allocate(size_t n)
    {
      table1_ = Vector< Node >();
      table2_ = Vector< Node >();

      for (size_t i = 0; i < n; ++i)
      {
        table1_.pushBack(Node{});
        table2_.pushBack(Node{});
      }
    }
  };
}

#endif

