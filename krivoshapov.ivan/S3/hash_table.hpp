#ifndef KRIVOSHAPOV_HASH_TABLE_HPP
#define KRIVOSHAPOV_HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace krivoshapov
{
  template <class Key, class Value, class Hash, class Equal>
  class HashTable
  {
    enum State
    {
      EMPTY,
      OCCUPIED,
      TOMB
    };

    struct Slot
    {
      State st;
      Key key;
      Value val;
      Slot() : st(EMPTY), key(), val() {}
    };

  public:
    explicit HashTable(size_t slots) : slots_(nullptr), cap_(slots < 1 ? 1 : slots), size_(0), tomb_(0)
    {
      slots_ = new Slot[cap_];
    }

    HashTable() : HashTable(8) {}

    ~HashTable() { delete[] slots_; }

    bool has(const Key &k) const
    {
      return findIndex(k) != cap_;
    }

    void add(const Key &k, const Value &v)
    {
      size_t fi = findIndex(k);
      if (fi != cap_)
      {
        slots_[fi].val = v;
        return;
      }
      size_t h = Hash()(k) % cap_;
      for (size_t step = 0; step < cap_; ++step)
      {
        size_t i = (h + step) % cap_;
        if (slots_[i].st != OCCUPIED)
        {
          bool wasTomb = slots_[i].st == TOMB;
          slots_[i].key = k;
          slots_[i].val = v;
          slots_[i].st = OCCUPIED;
          ++size_;
          if (wasTomb)
          {
            --tomb_;
          }
          return;
        }
      }
      throw std::length_error("hash table is full");
    }

    Value drop(const Key &k)
    {
      size_t i = findIndex(k);
      if (i == cap_)
      {
        throw std::out_of_range("key not found");
      }
      Value out = slots_[i].val;
      slots_[i].st = TOMB;
      --size_;
      ++tomb_;
      return out;
    }

    Value &at(const Key &k)
    {
      size_t i = findIndex(k);
      if (i == cap_)
      {
        throw std::out_of_range("key not found");
      }
      return slots_[i].val;
    }

    const Value &at(const Key &k) const
    {
      size_t i = findIndex(k);
      if (i == cap_)
      {
        throw std::out_of_range("key not found");
      }
      return slots_[i].val;
    }

    size_t size() const noexcept { return size_; }
    size_t slotCount() const noexcept { return cap_; }

  private:
    Slot *slots_;
    size_t cap_;
    size_t size_;
    size_t tomb_;

    size_t findIndex(const Key &k) const
    {
      size_t h = Hash()(k) % cap_;
      for (size_t step = 0; step < cap_; ++step)
      {
        size_t i = (h + step) % cap_;
        if (slots_[i].st == EMPTY)
        {
          return cap_;
        }
        if (slots_[i].st == OCCUPIED && Equal()(slots_[i].key, k))
        {
          return i;
        }
      }
      return cap_;
    }
  };
}

#endif
