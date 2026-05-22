#ifndef ROBIN_HOOD_HASH_HPP
#define ROBIN_HOOD_HASH_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <string>

namespace karpenko {

  struct StdHash {
    std::size_t operator()(const std::string& s) const
    {
      return std::hash<std::string>{}(s);
    }
  };

  template <typename Key, typename Value,
            typename Hash = StdHash,
            typename Equal = std::equal_to<Key>>
  class RobinHoodHashTable {
  public:
    using size_type = std::size_t;

  private:
    struct Entry {
      Key key_;
      Value value_;
      bool occupied_;
      size_type dist_;

      Entry():
        occupied_(false),
        dist_(0)
      {}

      Entry(const Key& k, const Value& v):
        key_(k),
        value_(v),
        occupied_(true),
        dist_(0)
      {}

      Entry(Key&& k, Value&& v):
        key_(std::move(k)),
        value_(std::move(v)),
        occupied_(true),
        dist_(0)
      {}

      Entry(Entry&& other) noexcept:
        key_(std::move(other.key_)),
        value_(std::move(other.value_)),
        occupied_(other.occupied_),
        dist_(other.dist_)
      {
        other.occupied_ = false;
      }

      Entry& operator=(Entry&& other) noexcept
      {
        if (this != &other) {
          key_ = std::move(other.key_);
          value_ = std::move(other.value_);
          occupied_ = other.occupied_;
          dist_ = other.dist_;
          other.occupied_ = false;
        }
        return *this;
      }

      Entry(const Entry&) = delete;
      Entry& operator=(const Entry&) = delete;
    };

    Entry* table_;
    size_type capacity_;
    size_type size_;
    Hash hasher_;
    Equal equal_;

    static constexpr float LOAD_FACTOR = 0.8f;
    static constexpr size_type DEFAULT_CAPACITY = 16;
    static constexpr size_type GROWTH_FACTOR = 2;

    size_type nextPowerOfTwo(size_type n) const
    {
      size_type p = 1;
      while (p < n) {
        p <<= 1;
      }
      return p;
    }

    void rehash(size_type new_cap)
    {
      RobinHoodHashTable new_table(new_cap, hasher_, equal_);
      for (size_type i = 0; i < capacity_; ++i) {
        if (table_[i].occupied_) {
          new_table.insert(std::move(table_[i].key_), std::move(table_[i].value_));
        }
      }
      swap(new_table);
    }

    void checkGrow()
    {
      if (static_cast<float>(size_) / capacity_ > LOAD_FACTOR) {
        rehash(capacity_ * GROWTH_FACTOR);
      }
    }

  public:
    RobinHoodHashTable(size_type initial_capacity = DEFAULT_CAPACITY, Hash h = Hash(), Equal e = Equal()):
      capacity_(nextPowerOfTwo(initial_capacity)),
      size_(0),
      hasher_(h),
      equal_(e)
    {
      table_ = new Entry[capacity_];
    }

    ~RobinHoodHashTable()
    {
      delete[] table_;
    }

    RobinHoodHashTable(const RobinHoodHashTable& other):
      capacity_(other.capacity_),
      size_(other.size_),
      hasher_(other.hasher_),
      equal_(other.equal_)
    {
      table_ = new Entry[capacity_];
      for (size_type i = 0; i < capacity_; ++i) {
        if (other.table_[i].occupied_) {
          table_[i].key_ = other.table_[i].key_;
          table_[i].value_ = other.table_[i].value_;
          table_[i].occupied_ = true;
          table_[i].dist_ = other.table_[i].dist_;
        } else {
          table_[i].occupied_ = false;
        }
      }
    }

    RobinHoodHashTable(RobinHoodHashTable&& other) noexcept:
      table_(other.table_),
      capacity_(other.capacity_),
      size_(other.size_),
      hasher_(std::move(other.hasher_)),
      equal_(std::move(other.equal_))
    {
      other.table_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    RobinHoodHashTable& operator=(const RobinHoodHashTable& other)
    {
      if (this != &other) {
        RobinHoodHashTable tmp(other);
        swap(tmp);
      }
      return *this;
    }

    RobinHoodHashTable& operator=(RobinHoodHashTable&& other) noexcept
    {
      if (this != &other) {
        delete[] table_;
        table_ = other.table_;
        capacity_ = other.capacity_;
        size_ = other.size_;
        hasher_ = std::move(other.hasher_);
        equal_ = std::move(other.equal_);
        other.table_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
      }
      return *this;
    }

    void swap(RobinHoodHashTable& other) noexcept
    {
      std::swap(table_, other.table_);
      std::swap(capacity_, other.capacity_);
      std::swap(size_, other.size_);
      std::swap(hasher_, other.hasher_);
      std::swap(equal_, other.equal_);
    }

    void insert(const Key& key, const Value& value)
    {
      if (contains(key)) {
        at(key) = value;
        return;
      }

      checkGrow();
      size_type idx = hasher_(key) & (capacity_ - 1);
      Entry new_entry(key, value);
      new_entry.dist_ = 0;

      while (true) {
        if (!table_[idx].occupied_) {
          table_[idx] = std::move(new_entry);
          ++size_;
          return;
        }
        if (new_entry.dist_ > table_[idx].dist_) {
          std::swap(new_entry, table_[idx]);
        }
        ++idx;
        ++new_entry.dist_;
        idx &= (capacity_ - 1);
      }
    }

    void insert(Key&& key, Value&& value)
    {
      if (contains(key)) {
        at(key) = std::move(value);
        return;
      }

      checkGrow();
      size_type idx = hasher_(key) & (capacity_ - 1);
      Entry new_entry(std::move(key), std::move(value));
      new_entry.dist_ = 0;

      while (true) {
        if (!table_[idx].occupied_) {
          table_[idx] = std::move(new_entry);
          ++size_;
          return;
        }
        if (new_entry.dist_ > table_[idx].dist_) {
          std::swap(new_entry, table_[idx]);
        }
        ++idx;
        ++new_entry.dist_;
        idx &= (capacity_ - 1);
      }
    }

    bool contains(const Key& key) const
    {
      size_type idx = hasher_(key) & (capacity_ - 1);
      size_type dist = 0;
      while (table_[idx].occupied_ && table_[idx].dist_ >= dist) {
        if (equal_(table_[idx].key_, key)) {
          return true;
        }
        ++idx;
        ++dist;
        idx &= (capacity_ - 1);
      }
      return false;
    }

    Value& at(const Key& key)
    {
      size_type idx = hasher_(key) & (capacity_ - 1);
      size_type dist = 0;
      while (table_[idx].occupied_ && table_[idx].dist_ >= dist) {
        if (equal_(table_[idx].key_, key)) {
          return table_[idx].value_;
        }
        ++idx;
        ++dist;
        idx &= (capacity_ - 1);
      }
      throw std::out_of_range("key not found");
    }

    const Value& at(const Key& key) const
    {
      size_type idx = hasher_(key) & (capacity_ - 1);
      size_type dist = 0;
      while (table_[idx].occupied_ && table_[idx].dist_ >= dist) {
        if (equal_(table_[idx].key_, key)) {
          return table_[idx].value_;
        }
        ++idx;
        ++dist;
        idx &= (capacity_ - 1);
      }
      throw std::out_of_range("key not found");
    }

    bool erase(const Key& key)
    {
      size_type idx = hasher_(key) & (capacity_ - 1);
      size_type dist = 0;
      while (table_[idx].occupied_ && table_[idx].dist_ >= dist) {
        if (equal_(table_[idx].key_, key)) {
          table_[idx].occupied_ = false;
          --size_;
          return true;
        }
        ++idx;
        ++dist;
        idx &= (capacity_ - 1);
      }
      return false;
    }

    size_type size() const
    {
      return size_;
    }

    bool empty() const
    {
      return size_ == 0;
    }

    class iterator {
    public:
      iterator(Entry* p = nullptr, Entry* e = nullptr)
        : ptr_(p),
          end_(e)
      {
        skip();
      }

      std::pair<const Key&, Value&> operator*() const
      {
        return {ptr_->key_, ptr_->value_};
      }

      iterator& operator++()
      {
        ++ptr_;
        skip();
        return *this;
      }

      iterator operator++(int)
      {
        iterator tmp = *this;
        ++ptr_;
        skip();
        return tmp;
      }

      bool operator==(const iterator& other) const
      {
        return ptr_ == other.ptr_;
      }

      bool operator!=(const iterator& other) const
      {
        return !(*this == other);
      }

    private:
      Entry* ptr_;
      Entry* end_;

      void skip()
      {
        while (ptr_ != end_ && !ptr_->occupied_) {
          ++ptr_;
        }
      }
    };

    class const_iterator {
    public:
      const_iterator(const Entry* p = nullptr, const Entry* e = nullptr)
        : ptr_(p),
          end_(e)
      {
        skip();
      }

      const_iterator(const iterator& it)
        : ptr_(it.ptr_),
          end_(it.end_)
      {
        skip();
      }

      std::pair<const Key&, const Value&> operator*() const
      {
        return {ptr_->key_, ptr_->value_};
      }

      const_iterator& operator++()
      {
        ++ptr_;
        skip();
        return *this;
      }

      const_iterator operator++(int)
      {
        const_iterator tmp = *this;
        ++ptr_;
        skip();
        return tmp;
      }

      bool operator==(const const_iterator& other) const
      {
        return ptr_ == other.ptr_;
      }

      bool operator!=(const const_iterator& other) const
      {
        return !(*this == other);
      }

    private:
      const Entry* ptr_;
      const Entry* end_;

      void skip()
      {
        while (ptr_ != end_ && !ptr_->occupied_) {
          ++ptr_;
        }
      }
    };

    iterator begin()
    {
      return iterator(table_, table_ + capacity_);
    }

    iterator end()
    {
      return iterator(table_ + capacity_, table_ + capacity_);
    }

    const_iterator begin() const
    {
      return const_iterator(table_, table_ + capacity_);
    }

    const_iterator end() const
    {
      return const_iterator(table_ + capacity_, table_ + capacity_);
    }
  };

}

#endif
