#ifndef ALEKSEEV_S3_HASH_TABLE_HPP
#define ALEKSEEV_S3_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace alekseev
{
  enum class SlotState
  {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    explicit HashTable(size_t slots = 101, Hash hash = Hash(), Equal equal = Equal()):
      slots_(nullptr),
      size_(0),
      tombstones_(0),
      capacity_(slots == 0 ? 1 : slots),
      hash_(hash),
      equal_(equal)
    {
      slots_ = new Slot[capacity_];
    }

    HashTable(const HashTable& other):
      HashTable(other.capacity_, other.hash_, other.equal_)
    {
      try
      {
        for (size_t i = 0; i < other.capacity_; ++i)
        {
          if (other.slots_[i].state == SlotState::OCCUPIED)
          {
            add(other.slots_[i].key, other.slots_[i].value);
          }
        }
      }
      catch (...)
      {
        delete[] slots_;
        slots_ = nullptr;
        throw;
      }
    }

    HashTable(HashTable&& other) noexcept:
      slots_(other.slots_),
      size_(other.size_),
      tombstones_(other.tombstones_),
      capacity_(other.capacity_),
      hash_(std::move(other.hash_)),
      equal_(std::move(other.equal_))
    {
      other.slots_ = nullptr;
      other.size_ = 0;
      other.tombstones_ = 0;
      other.capacity_ = 0;
    }

    HashTable& operator=(const HashTable& other)
    {
      if (this != &other)
      {
        HashTable copy(other);
        swap(copy);
      }
      return *this;
    }

    HashTable& operator=(HashTable&& other) noexcept
    {
      if (this != &other)
      {
        delete[] slots_;
        slots_ = other.slots_;
        size_ = other.size_;
        tombstones_ = other.tombstones_;
        capacity_ = other.capacity_;
        hash_ = std::move(other.hash_);
        equal_ = std::move(other.equal_);
        other.slots_ = nullptr;
        other.size_ = 0;
        other.tombstones_ = 0;
        other.capacity_ = 0;
      }
      return *this;
    }

    ~HashTable()
    {
      delete[] slots_;
    }

    void add(const Key& key, const Value& value)
    {
      insert_new(key, value);
    }

    void add(Key&& key, Value&& value)
    {
      insert_new(std::move(key), std::move(value));
    }

    void insert_or_assign(const Key& key, const Value& value)
    {
      Slot* slot = find_slot(key);
      if (slot != nullptr)
      {
        slot->value = value;
        return;
      }
      add(key, value);
    }

    bool has(const Key& key) const
    {
      return find_slot(key) != nullptr;
    }

    Value& at(const Key& key)
    {
      Slot* slot = find_slot(key);
      if (slot == nullptr)
      {
        throw std::out_of_range("hash key");
      }
      return slot->value;
    }

    const Value& at(const Key& key) const
    {
      const Slot* slot = find_slot(key);
      if (slot == nullptr)
      {
        throw std::out_of_range("hash key");
      }
      return slot->value;
    }

    Value drop(const Key& key)
    {
      Slot* slot = find_slot(key);
      if (slot == nullptr)
      {
        throw std::out_of_range("hash key");
      }
      Value value(std::move(slot->value));
      slot->state = SlotState::TOMBSTONE;
      --size_;
      ++tombstones_;
      return value;
    }

    void rehash(size_t slots)
    {
      HashTable next(slots == 0 ? 1 : slots, hash_, equal_);
      for (size_t i = 0; i < capacity_; ++i)
      {
        if (slots_[i].state == SlotState::OCCUPIED)
        {
          next.add(slots_[i].key, slots_[i].value);
        }
      }
      swap(next);
    }

    void clear()
    {
      for (size_t i = 0; i < capacity_; ++i)
      {
        slots_[i].state = SlotState::EMPTY;
      }
      size_ = 0;
      tombstones_ = 0;
    }

    size_t size() const noexcept
    {
      return size_;
    }

    size_t capacity() const noexcept
    {
      return capacity_;
    }

  private:
    struct Slot
    {
      Slot():
        state(SlotState::EMPTY),
        key(),
        value()
      {}

      SlotState state;
      Key key;
      Value value;
    };

    template< class K, class V >
    void insert_new(K&& key, V&& value)
    {
      if (capacity_ == 0)
      {
        throw std::overflow_error("hash table capacity");
      }

      Slot* first_tombstone = nullptr;
      const size_t base = hash_(key) % capacity_;
      for (size_t i = 0; i < capacity_; ++i)
      {
        Slot& slot = slots_[(base + i * i) % capacity_];
        if (slot.state == SlotState::OCCUPIED)
        {
          if (equal_(slot.key, key))
          {
            throw std::logic_error("duplicate hash key");
          }
        }
        else if (slot.state == SlotState::TOMBSTONE)
        {
          if (first_tombstone == nullptr)
          {
            first_tombstone = &slot;
          }
        }
        else
        {
          Slot& target = first_tombstone == nullptr ? slot : *first_tombstone;
          target.key = std::forward< K >(key);
          target.value = std::forward< V >(value);
          if (target.state == SlotState::TOMBSTONE)
          {
            --tombstones_;
          }
          target.state = SlotState::OCCUPIED;
          ++size_;
          return;
        }
      }

      if (first_tombstone != nullptr)
      {
        first_tombstone->key = std::forward< K >(key);
        first_tombstone->value = std::forward< V >(value);
        first_tombstone->state = SlotState::OCCUPIED;
        ++size_;
        --tombstones_;
        return;
      }
      throw std::overflow_error("hash table full");
    }

    Slot* find_slot(const Key& key)
    {
      return const_cast< Slot* >(static_cast< const HashTable& >(*this).find_slot(key));
    }

    const Slot* find_slot(const Key& key) const
    {
      if (capacity_ == 0)
      {
        return nullptr;
      }
      const size_t base = hash_(key) % capacity_;
      for (size_t i = 0; i < capacity_; ++i)
      {
        const Slot& slot = slots_[(base + i * i) % capacity_];
        if (slot.state == SlotState::EMPTY)
        {
          return nullptr;
        }
        if (slot.state == SlotState::OCCUPIED && equal_(slot.key, key))
        {
          return &slot;
        }
      }
      return nullptr;
    }

    void swap(HashTable& other) noexcept
    {
      using std::swap;
      swap(slots_, other.slots_);
      swap(size_, other.size_);
      swap(tombstones_, other.tombstones_);
      swap(capacity_, other.capacity_);
      swap(hash_, other.hash_);
      swap(equal_, other.equal_);
    }

    Slot* slots_;
    size_t size_;
    size_t tombstones_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;
  };
}

#endif
