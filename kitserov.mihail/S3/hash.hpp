#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>
namespace kitserov
{
 
  template< class Key, class Value, class Hash, class Equal >
  class HashTable
  {
    struct Slot
    {
      Key key_;
      Value val_;
      bool fill_;
    };
 	  size_t size_;
    size_t capacity_;
    std::vector< Slot > slots_;
    size_t hash_k(const Key& key) const 
    {
        return Hash{}(key) % capacity_;
    }
    size_t probe(const Key& key, size_t i) const
    {
      return (hash_k(key) + i * i) % capacity_;
    }
    static constexpr size_t DEFAULT_CAPACITY = 8;
  public:
    HashTable() : size_(0), capacity_(DEFAULT_CAPACITY) {}
    HashTable(size_t size) : size_(0), capacity_(size), slots_(capacity_) {}
    HashTable(const HashTable& other)
    {
      copy(other);
    }
    HashTable(HashTable&& other) noexcept
    {
      swap(other);
    }
    HashTable& operator=(const HashTable& other)
    {
      copy(other);
      return *this;
    }
    HashTable& operator=(HashTable&& other) noexcept
    {
      swap(other);
      return *this;
    }

    bool isEmpty()
    {
      return size_ == 0;
    }

    void copy(const HashTable& other)
    {
      if (this == &other) {
        return;
      }
      HashTable tmp(other.capacity_);
      try {
        tmp.slots_ = other.slots_;
      } catch (...) {
        throw;
      }
      tmp.size_ = other.size_;
      swap(tmp);
    }

    void swap(HashTable& other) noexcept
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(slots_, other.slots_);
    }

    Value* find(const Key& key) noexcept
    {
      for (size_t i = 0; i < capacity_; ++i) {
        size_t idx = probe(key, i);
        if (!slots_[idx].fill_) {
            return nullptr;
        }
        if (Equal{}(slots_[idx].key_, key)) {
            return &slots_[idx].val_;
        }
      }
      return nullptr;
    }
    const Value* find(const Key& key) const noexcept
    {
      return find(key);
    }

    bool contains(const Key& key) const {
        return find(key) != nullptr;
    }


    bool add(const Key& key, const Value& value)
    {
      if (loadFactor() >= 0.8f) {
        throw;
      }
      for (size_t i = 0; i < capacity_; ++i) {
        size_t idx = probe(key, i);
        if (!slots_[idx].fill_) {
            slots_[idx].key_ = key;
            slots_[idx].val_ = value;
            slots_[idx].fill_ = true;
            ++size_;
            return true;
        }
        if (Equal{}(slots_[idx].key_, key)) {
            slots_[idx].val_ = value;
            return true;
        }
      }
      return false;
    }

    Value& operator[](const Key& key) {
      return *find(key);
    }

    bool erase(const Key& key) {
      for (size_t i = 0; i < capacity_; ++i) {
        size_t idx = probe(key, i);
        if (!slots_[idx].fill_) {
          return false;
        }
        if (Equal{}(slots_[idx].key_, key)) {
          slots_[idx].fill_ = false;
          --size_;
          return true;
        }
      }
      return false;
    }


    float loadFactor()
    {
      if (capacity_ == 0) {
        return 1;
      }
      return size_ / capacity_;
    }

    size_t size()
    {
      return size_;
    }
    size_t capacity()
    {
      return capacity_;
    }
    void rehash(size_t new_capacity)
    {
      std::vector<Slot> new_slots(new_capacity);
      for (const Slot& slot : slots_) {
        try {
          if (slot.fill_) {
            size_t i = 0;
            size_t idx = probe(slot.key, i);
            while (new_slots[idx].fill_) {
                ++i;
                idx = probe(slot.key, i);
            }
            new_slots[idx] = slot;
          }
        } catch (...) {
          throw;
        }
      }
      slots_ = std::move(new_slots);
      capacity_ = new_capacity;
    }
    class iterator
    {
      friend class HashTable;
      const HashTable* table_ = nullptr;
      size_t idx_;
      iterator(const HashTable* t, size_t idx) noexcept : table_(t), idx_(idx) {}
    public:
      iterator() noexcept = default;
      Value& operator*() const
      {
          return table_ -> slots_[idx_].val_;
      }
      Value* operator->() const
      {
          return &(table_ -> slots_[idx_].val_);
      }
      iterator& operator++() noexcept
      {
        size_t cap = table_ -> capacity();
        while (++idx_ < cap) {
          if (table_ -> slots_[idx_].fill_)
            break;
        }
        return *this;
      }
      iterator operator++(int) noexcept
      {
        iterator tmp = *this;
        ++(*this);
        return tmp;
      }
      bool operator==(const iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ == o.idx_;
      }
      bool operator!=(const iterator& o) const noexcept
      {
        return !(*this == o);
      }
      bool operator<(const iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ < o.idx_;
      }
      bool operator>(const iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ > o.idx_;
      }
    };
    class const_iterator
    {
      friend class HashTable;
      const HashTable* table_ = nullptr;
      size_t idx_;
      const_iterator(const HashTable* t, size_t idx) noexcept : table_(t), idx_(idx) {}
    public:
      const_iterator() noexcept = default;
      const_iterator(const iterator& it) noexcept : m_table(it.table_), m_idx(it.idx_) {}

      const Value& operator*() const
      {
          return table_ -> slots_[idx_].val_;
      }
      const Value* operator->() const
      {
          return &(table_ -> slots_[idx_].val_);
      }
      const_iterator& operator++() noexcept
      {
        size_t cap = table_ -> capacity();
        while (++idx_ < cap) {
          if (table_ -> slots_[idx_].fill_)
            break;
        }
        return *this;
      }
      const_iterator operator++(int) noexcept
      {
        const_iterator tmp = *this;
        ++(*this);
        return tmp;
      }
      bool operator==(const const_iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ == o.idx_;
      }
      bool operator!=(const const_iterator& o) const noexcept
      {
        return !(*this == o);
      }
      bool operator<(const const_iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ < o.idx_;
      }
      bool operator>(const const_iterator& o) const noexcept
      {
        return table_ == o.table_ && idx_ > o.idx_;
      }
    };

    iterator begin() const noexcept
    {
      for (size_t i = 0; i < capacity_; ++i) {
        if (slots_[i].fill_) {
          return iterator(this, i);
        }
      }
      return end();
    }
    iterator end() noexcept { return iterator(this, capacity()); }

    const_iterator begin() const noexcept
    {
      for (size_t i = 0; i < capacity_; ++i) {
        if (slots_[i].fill_) {
          return const_iterator(this, i);
        }
      }
      return end();
    }
    const_iterator end() noexcept { return const_iterator(this, capacity()); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

  };
  template< class T >
  struct SipHash {
      size_t operator()(const T& key) const 
      {
          boost::hash2::siphash_64 hash;  
          boost::hash2::hash_append(hash, {}, key);
          return hash.result();
      }
  };
}
