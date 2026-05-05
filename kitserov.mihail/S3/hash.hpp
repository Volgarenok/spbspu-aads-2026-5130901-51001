#include <vector>
#include <utility>
namespace kitserov
{
  using empty = 0;
  using occupied = 1;
  using tombstone = 2;
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    HashTable() : size_(0), capacity_(0) {}
    HashTable(size_t size) : size_(0), capacity_(size)
    {
      slots_.resize(capacity);
      for (size_t i = 0; i < capacity_; ++i) {
        slots[i].second = empty;
      }
    }
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
      swap(other)
      return *this
    }

    void copy(const HashTable& other)
    {
      if (this == &other) {
        return
      }
      HashTable tmp(capacity_);
      try {
        tmp.slots_ = other.slots_;
      } catch (...) {
        throw;
      }
      swap(tmp);
    }

    void swap(HashTable& other) noexcept
    {
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
      std::swap(slots_, other.slots_);
    }

    float loadFactor()
    {
      if (capacity_ == 0) {
        return 1f;
      }
      return size_ / capacity_;
    }
  private:
    size_t size_;
    size_t capacity_;
    std::vector< std::pair < Value, int > > slots_;
    
    size_t probe(const Key& k, size_t i) 
    {
      return (Hash(k) + i * i) % capacity_;
    }
  };
}