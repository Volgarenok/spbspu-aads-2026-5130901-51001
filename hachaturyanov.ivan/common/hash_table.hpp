#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <stdexcept>
#include <utility>
#include "../common/list.hpp"


namespace hachaturyanov
{
  static constexpr size_t DEFAULT_CAPACITY = 17;

  inline bool isPrime(size_t n) {
    if (n <= 1) {
      return false;
    }
    if (n <= 3) {
      return true;
    }
    if (n % 2 == 0 || n % 3 == 0) {
      return false;
    }
    for (size_t i = 5; i * i <= n; i += 6) {
      if (n % i == 0 || n % (i + 2) == 0) {
        return false;
      }
    }
    return true;
  }

  inline size_t nextPrime(size_t n) {
    if (n <= 2) {
      return 2;
    }
    if (n % 2 == 0) {
      n++;
    } else {
      n += 2;
    }
    while (!isPrime(n)) {
      n += 2;
    }
    return n;
  }

  enum State {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template< class Key, class Value > struct Slot {
    Key key;
    Value value;
    State state = EMPTY;
  };

  template< class Key, class Value > class HTIter;
  template< class Key, class Value > class HTCIter;

  template< class Key, class Value, class Hash, class Equal >
  class HashTable {
    friend class HTIter< Key, Value >;
    friend class HTCIter< Key, Value >;
   public:
    HashTable();
    HashTable(size_t slots);
    ~HashTable();

    void add(const Key &key, const Value &value);
    Value drop(const Key &key);
    bool has(const Key &key) const;
    void rehash(size_t slots);

    size_t size() const;
    size_t capacity() const;
    bool empty() const;
    void clear();
    const Value & get(const Key &key) const;
    Value & get(const Key &key);

    Value & operator[](const Key &key);

    List< Key > keys() const;

    HTIter< Key, Value > begin();
    HTIter< Key, Value > end();
    HTCIter< Key, Value > begin() const;
    HTCIter< Key, Value > end() const;
    HTCIter< Key, Value > cbegin() const;
    HTCIter< Key, Value > cend() const;

    HashTable(const HashTable &other);
    HashTable(HashTable &&other) noexcept;
    HashTable &operator=(const HashTable &other);
    HashTable &operator=(HashTable &&other) noexcept;
   private:
    Slot< Key, Value >* data_;
    size_t size_;
    size_t capacity_;
    Hash hash_;
    Equal equal_;

    void swap_(HashTable &other);
    size_t findIndex_(const Key &key) const;
  };

  template< class Key, class Value > class HTIter {
    template< class K, class V, class H, class E > friend class HashTable;
   public:
    using value_type = std::pair< const Key &, Value & >;

    HTIter & operator++();
    value_type operator*();
    bool operator==(const HTIter &other) const;
    bool operator!=(const HTIter &other) const;

   private:
    HTIter(Slot< Key, Value >* data, size_t index, size_t capacity);

    Slot< Key, Value >* data_;
    size_t index_;
    size_t capacity_;

  };

  template< class Key, class Value > class HTCIter {
    template< class K, class V, class H, class E > friend class HashTable;
    friend class HTIter< Key, Value >;
   public:
    using value_type = std::pair< const Key &, const Value & >;

    HTCIter & operator++();
    value_type operator*() const;
    bool operator==(const HTCIter &other) const;
    bool operator!=(const HTCIter &other) const;

    HTCIter(const HTIter< Key, Value > &other);

   private:
    HTCIter(const Slot< Key, Value >* data, size_t index, size_t capacity);

    const Slot< Key, Value >* data_;
    size_t index_;
    size_t capacity_;
  };

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::cend() const
  {
    return end();
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::cbegin() const
  {
    return begin();
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end() const
  {
    return HTCIter< Key, Value >(data_, capacity_, capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTCIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin() const
  {
    size_t i = 0;
    while(i < capacity_ && data_[i].state != OCCUPIED) {
      i++;
    }
    return HTCIter< Key, Value >(data_, i, capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value > HashTable< Key, Value, Hash, Equal >::end()
  {
    return HTIter< Key, Value >(data_, capacity_, capacity_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HTIter< Key, Value > HashTable< Key, Value, Hash, Equal >::begin()
  {
    size_t i = 0;
    while(i < capacity_ && data_[i].state != OCCUPIED) {
      i++;
    }
    return HTIter< Key, Value >(data_, i, capacity_);
  }

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter(const Slot< Key, Value >* data, size_t index, size_t capacity):
   data_(data),
   index_(index),
   capacity_(capacity)
  {}

  template< class Key, class Value >
  HTCIter< Key, Value >::HTCIter(const HTIter< Key, Value > &other):
   data_(other.data_),
   index_(other.index_),
   capacity_(other.capacity_)
  {}

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator!=(const HTCIter &other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  bool HTCIter< Key, Value >::operator==(const HTCIter &other) const
  {
    return index_ == other.index_;
  }

  template< class Key, class Value >
  typename HTCIter< Key, Value >::value_type HTCIter< Key, Value >::operator*() const
  {
    return { data_[index_].key, data_[index_].value };
  }

  template< class Key, class Value >
  HTCIter< Key, Value > & HTCIter< Key, Value >::operator++()
  {
    do {
      index_++;
    } while (index_ < capacity_ && data_[index_].state != OCCUPIED);
    return *this;
  }

  template< class Key, class Value >
  HTIter< Key, Value >::HTIter(Slot< Key, Value >* data, size_t index, size_t capacity):
   data_(data),
   index_(index),
   capacity_(capacity)
  {}

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator!=(const HTIter &other) const
  {
    return !(*this == other);
  }

  template< class Key, class Value >
  bool HTIter< Key, Value >::operator==(const HTIter &other) const
  {
    return index_ == other.index_;
  }

  template< class Key, class Value >
  typename HTIter< Key, Value >::value_type HTIter< Key, Value >::operator*()
  {
    return { data_[index_].key, data_[index_].value };
  }

  template< class Key, class Value >
  HTIter< Key, Value > & HTIter< Key, Value >::operator++()
  {
    do {
      index_++;
    } while (index_ < capacity_ && data_[index_].state != OCCUPIED);
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  List< Key > HashTable< Key, Value, Hash, Equal >::keys() const
  {
    List< Key > result;
    for (size_t i = 0; i < capacity_; i++) {
      if (data_[i].state == OCCUPIED) {
        result.insertSorted(data_[i].key);
      }
    }
    return result;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >
  &HashTable< Key, Value, Hash, Equal >::operator=(HashTable &&other) noexcept
  {
    if (this != &other) {
      swap_(other);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  const Value & HashTable< Key, Value, Hash, Equal >::get(const Key &key) const
  {
    size_t id = findIndex_(key);
    if (id == capacity_ || data_[id].state != OCCUPIED) {
      throw std::runtime_error("Key not found");
    }
    return data_[id].value;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::get(const Key &key)
  {
    size_t id = findIndex_(key);
    if (id == capacity_ || data_[id].state != OCCUPIED) {
      throw std::runtime_error("Key not found");
    }
    return data_[id].value;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value & HashTable< Key, Value, Hash, Equal >::operator[](const Key &key)
  {
    size_t id = findIndex_(key);
    if (id == capacity_) {
      throw std::runtime_error("Failed to add key-value pair, rehashing recommended");
    }
    if (data_[id].state != OCCUPIED) {
      data_[id].key = key;
      data_[id].value = Value{};
      data_[id].state = OCCUPIED;
      size_++;
    }
    return data_[id].value;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::findIndex_(const Key &key) const
  {
    size_t tombstone_id = capacity_;
    for (size_t i = 0; i < capacity_; i++) {
      size_t id = (hash_(key) + i * i) % capacity_;
      const Slot< Key, Value > &slot = data_[id];
      if (slot.state == OCCUPIED && equal_(slot.key, key)) {
        return id;
      } else if (slot.state == TOMBSTONE && tombstone_id == capacity_) {
        tombstone_id = id;
      } else if (slot.state == EMPTY) {
        if (tombstone_id != capacity_) {
          return tombstone_id;
        } else {
          return id;
        }
      }
    }
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >
  &HashTable< Key, Value, Hash, Equal >::operator=(const HashTable &other)
  {
    if (this != &other) {
      HashTable< Key, Value, Hash, Equal > temp(other);
      swap_(temp);
    }
    return *this;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(HashTable &&other) noexcept:
   data_(other.data_),
   size_(other.size_),
   capacity_(other.capacity_),
   hash_(other.hash_),
   equal_(other.equal_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable &other):
   data_(new Slot< Key, Value >[other.capacity_]),
   size_(other.size_),
   capacity_(other.capacity_),
   hash_(other.hash_),
   equal_(other.equal_)
  {
    for (size_t i = 0; i < capacity_; i++) {
      data_[i] = other.data_[i];
    }
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::size() const
  {
    return size_;
  }

  template< class Key, class Value, class Hash, class Equal >
  size_t HashTable< Key, Value, Hash, Equal >::capacity() const
  {
    return capacity_;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::empty() const
  {
    return size_ == 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::clear()
  {
    for (size_t i = 0; i < capacity_; i++) {
      data_[i].state = EMPTY;
    }
    size_ = 0;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::swap_(HashTable &other)
  {
    std::swap(data_, other.data_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
    std::swap(hash_, other.hash_);
    std::swap(equal_, other.equal_);
  }

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable():
   data_(new Slot< Key, Value >[nextPrime(DEFAULT_CAPACITY)]),
   size_(0),
   capacity_(nextPrime(DEFAULT_CAPACITY))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::HashTable(size_t slots):
   data_(new Slot< Key, Value >[nextPrime(slots)]),
   size_(0),
   capacity_(nextPrime(slots))
  {}

  template< class Key, class Value, class Hash, class Equal >
  HashTable< Key, Value, Hash, Equal >::~HashTable()
  {
    delete[] data_;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::add(const Key &key, const Value &value)
  {
    size_t id = findIndex_(key);
    if (id == capacity_) {
      throw std::runtime_error("Failed to add key-value pair, rehashing recommended");
    }
    if (data_[id].state != OCCUPIED) {
      data_[id].key = key;
      data_[id].state = OCCUPIED;
      size_++;
    }
    data_[id].value = value;
  }

  template< class Key, class Value, class Hash, class Equal >
  Value HashTable< Key, Value, Hash, Equal >::drop(const Key &key)
  {
    size_t id = findIndex_(key);
    if (id == capacity_ || data_[id].state != OCCUPIED) {
      throw std::runtime_error("Key not found");
    }
    Value value = data_[id].value;
    data_[id].state = TOMBSTONE;
    size_--;
    return value;
  }

  template< class Key, class Value, class Hash, class Equal >
  bool HashTable< Key, Value, Hash, Equal >::has(const Key &key) const
  {
    size_t id = findIndex_(key);
    return id != capacity_ && data_[id].state == OCCUPIED;
  }

  template< class Key, class Value, class Hash, class Equal >
  void HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
  {
    size_t new_capacity = nextPrime(slots);
    Slot< Key, Value >* new_data = new Slot< Key, Value >[new_capacity];

    for (size_t i = 0; i < capacity_; i++) {
      if (data_[i].state == OCCUPIED) {
        for (size_t j = 0; j < new_capacity; j++) {
          size_t id = (hash_(data_[i].key) + j * j) % new_capacity;
          if (new_data[id].state == EMPTY) {
            new_data[id] = data_[i];
            break;
          }
        }
      }
    }

    std::swap(data_, new_data);
    std::swap(capacity_, new_capacity);
    delete[] new_data;
  }

}

#endif
