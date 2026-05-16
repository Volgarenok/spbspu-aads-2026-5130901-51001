#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <functional>
#include <iterator>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace loseva {

template< class Key, class Value, class Hash, class Equal >
class HashTable {
public:
  using key_type = Key;
  using mapped_type = Value;
  using size_type = std::size_t;

private:
  enum class SlotState {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  struct Slot {
    SlotState state = SlotState::EMPTY;
    Key key;
    Value value;
  };

  std::vector< Slot > slots_;
  size_type size_;
  Hash hasher_;
  Equal equal_;

  size_type findOccupied(const Key & k) const
  {
    const size_type cap = slots_.size();
    if (cap == 0) {
      throw std::out_of_range("Key not found: table is empty");
    }
    const size_type h = hasher_(k) % cap;
    for (size_type i = 0; i < cap; ++i) {
      const size_type idx = (h + i * i) % cap;
      const auto & slot = slots_[idx];
      if (slot.state == SlotState::EMPTY) {
        throw std::out_of_range("Key not found");
      }
      if (slot.state == SlotState::OCCUPIED && equal_(slot.key, k)) {
        return idx;
      }
    }
    throw std::out_of_range("Key not found");
  }

public:

  class Entry {
  public:
    Entry(const Key & k, Value & v):
      key_(k),
      value_(v)
    {}

    const Key & key() const
    {
      return key_;
    }

    Value & value() const
    {
      return value_;
    }

  private:
    const Key & key_;
    Value & value_;
  };

  class ConstEntry {
  public:
    ConstEntry(const Key & k, const Value & v):
      key_(k),
      value_(v)
    {}

    const Key & key() const
    {
      return key_;
    }

    const Value & value() const
    {
      return value_;
    }

  private:
    const Key & key_;
    const Value & value_;
  };

  class iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    iterator(std::vector< Slot > * slots, size_type pos):
      slots_(slots),
      pos_(pos),
      entry_(nullptr)
    {
      advance();
      updateEntry();
    }

    Entry operator*() const
    {
      auto & slot = (*slots_)[pos_];
      return Entry(slot.key, slot.value);
    }

    struct Proxy {
      explicit Proxy(Entry e):
        entry_(e)
      {}
      Entry * operator->()
      {
        return &entry_;
      }
      Entry entry_;
    };

    Proxy operator->() const
    {
      auto & slot = (*slots_)[pos_];
      return Proxy(Entry(slot.key, slot.value));
    }

    iterator & operator++()
    {
      ++pos_;
      advance();
      updateEntry();
      return *this;
    }

    bool operator==(const iterator & other) const
    {
      return pos_ == other.pos_;
    }

    bool operator!=(const iterator & other) const
    {
      return !(*this == other);
    }

  private:
    std::vector< Slot > * slots_;
    size_type pos_;
    Entry * entry_;

    void advance()
    {
      while (pos_ < slots_->size()
        && (*slots_)[pos_].state != SlotState::OCCUPIED) {
        ++pos_;
      }
    }

    void updateEntry()
    {
      entry_ = nullptr;
    }
  };

  class const_iterator {
  public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;

    const_iterator(const std::vector< Slot > * slots, size_type pos):
      slots_(slots),
      pos_(pos)
    {
      advance();
    }

    ConstEntry operator*() const
    {
      const auto & slot = (*slots_)[pos_];
      return ConstEntry(slot.key, slot.value);
    }

    struct Proxy {
      explicit Proxy(ConstEntry e):
        entry_(e)
      {}
      ConstEntry * operator->()
      {
        return &entry_;
      }
      ConstEntry entry_;
    };

    Proxy operator->() const
    {
      const auto & slot = (*slots_)[pos_];
      return Proxy(ConstEntry(slot.key, slot.value));
    }

    const_iterator & operator++()
    {
      ++pos_;
      advance();
      return *this;
    }

    bool operator==(const const_iterator & other) const
    {
      return pos_ == other.pos_;
    }

    bool operator!=(const const_iterator & other) const
    {
      return !(*this == other);
    }

  private:
    const std::vector< Slot > * slots_;
    size_type pos_;

    void advance()
    {
      while (pos_ < slots_->size()
        && (*slots_)[pos_].state != SlotState::OCCUPIED) {
        ++pos_;
      }
    }
  };

  explicit HashTable(size_type initialSlots = 16):
    slots_(initialSlots),
    size_(0),
    hasher_(),
    equal_()
  {}

  HashTable(const HashTable &) = default;
  HashTable(HashTable &&) = default;
  HashTable & operator=(const HashTable &) = default;
  HashTable & operator=(HashTable &&) = default;

  void add(const Key & k, const Value & v)
  {
    if (has(k)) {
      throw std::logic_error("Key already exists");
    }
    const size_type cap = slots_.size();
    if (cap == 0) {
      throw std::overflow_error("Hash table is full");
    }
    const size_type h = hasher_(k) % cap;
    size_type tombstoneIdx = cap;
    for (size_type i = 0; i < cap; ++i) {
      const size_type idx = (h + i * i) % cap;
      auto & slot = slots_[idx];
      if (slot.state == SlotState::EMPTY) {
        const size_type insertIdx = (tombstoneIdx < cap) ? tombstoneIdx : idx;
        slots_[insertIdx].state = SlotState::OCCUPIED;
        slots_[insertIdx].key = k;
        slots_[insertIdx].value = v;
        ++size_;
        return;
      }
      if (slot.state == SlotState::TOMBSTONE && tombstoneIdx == cap) {
        tombstoneIdx = idx;
      }
    }
    if (tombstoneIdx < cap) {
      slots_[tombstoneIdx].state = SlotState::OCCUPIED;
      slots_[tombstoneIdx].key = k;
      slots_[tombstoneIdx].value = v;
      ++size_;
      return;
    }
    throw std::overflow_error("Hash table is full");
  }

  void insert_or_assign(const Key & k, const Value & v)
  {
    if (has(k)) {
      slots_[findOccupied(k)].value = v;
      return;
    }
    add(k, v);
  }

  Value drop(const Key & k)
  {
    const size_type idx = findOccupied(k);
    Value val = slots_[idx].value;
    slots_[idx].state = SlotState::TOMBSTONE;
    --size_;
    return val;
  }

  bool has(const Key & k) const
  {
    const size_type cap = slots_.size();
    if (cap == 0) {
      return false;
    }
    const size_type h = hasher_(k) % cap;
    for (size_type i = 0; i < cap; ++i) {
      const size_type idx = (h + i * i) % cap;
      const auto & slot = slots_[idx];
      if (slot.state == SlotState::EMPTY) {
        return false;
      }
      if (slot.state == SlotState::OCCUPIED && equal_(slot.key, k)) {
        return true;
      }
    }
    return false;
  }

  Value & at(const Key & k)
  {
    return slots_[findOccupied(k)].value;
  }

  const Value & at(const Key & k) const
  {
    return slots_[findOccupied(k)].value;
  }

  Value & get(const Key & k)
  {
    return at(k);
  }

  const Value & get(const Key & k) const
  {
    return at(k);
  }

  void rehash(size_type newSlots)
  {
    std::vector< Slot > oldSlots = std::move(slots_);
    slots_.assign(newSlots, Slot{});
    size_ = 0;
    for (const auto & slot : oldSlots) {
      if (slot.state == SlotState::OCCUPIED) {
        add(slot.key, slot.value);
      }
    }
  }

  void clear()
  {
    slots_.assign(slots_.size(), Slot{});
    size_ = 0;
  }

  size_type size() const
  {
    return size_;
  }

  size_type capacity() const
  {
    return slots_.size();
  }

  iterator begin()
  {
    return iterator(&slots_, 0);
  }

  iterator end()
  {
    return iterator(&slots_, slots_.size());
  }

  const_iterator begin() const
  {
    return const_iterator(&slots_, 0);
  }

  const_iterator end() const
  {
    return const_iterator(&slots_, slots_.size());
  }

  const_iterator cbegin() const
  {
    return begin();
  }

  const_iterator cend() const
  {
    return end();
  }
};

struct StringEqual {
  bool operator()(const std::string & a, const std::string & b) const
  {
    return a == b;
  }
};

}

#endif
