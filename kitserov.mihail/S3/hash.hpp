#include <vector>
#include <utility>
#include <cstddef>
#include <stdexcept>
namespace kitserov
{
  
  template< class Key, class Value, class Hash, class Equal >
  struct HashTable
  {
    using Slot = std::pair < Value, bool >;
    HashTable() : size_(0), capacity_(0) {}
    HashTable(size_t size) : size_(0), capacity_(size)
    {
      slots_.resize(capacity_);
      for (size_t i = 0; i < capacity_; ++i) {
        slots_[i].second = false;
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
      swap(other);
      return *this;
    }

    bool isEmpty()
    {
      return slots_.empty();
    }

    void copy(const HashTable& other)
    {
      if (this == &other) {
        return;
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

    Slot* find(const Value& val)
    {
      Slot home = slots_[Hash(val) % capacity_];
      size_t i = 1;
      while (!home.second || !(i < capacity_)) {
        if (Equal(val, home.first)) {
          return &home;
        }
        home = slots_[probe(val), i];
        ++i;
      }
      return nullptr;
    }

    void add(const Value& value)
    {
      if (loadFactor() >= 0.9f) {
        throw std::out_of_range();
      }
      Slot* home = find(value);
      if (home) {
        home -> first = value;
        home -> second = true;
        return;
      }
      throw;
    }

    float loadFactor()
    {
      if (capacity_ == 0) {
        return 1;
      }
      return size_ / capacity_;
    }
  private:
    size_t size_;
    size_t capacity_;
    std::vector< Slot > slots_;
    
    size_t probe(const Value& val, size_t i) 
    {
      return (Hash(val) + i * i) % capacity_;
    }
    void rehash(size_t new_capacity)
    {
      std::vector< Slot > old_slots = std::move(slots_);
      size_t old_capacity = capacity_;
      capacity_ = new_capacity;
      slots_.resize(capacity_);
      size_ = 0;
      for (size_t i = 0; i < old_capacity; ++i) {

      }
    }
  };

  class siphash_64
  {
  public:

    using result_type = std::uint64_t;

    constexpr siphash_64();
    explicit constexpr siphash_64( std::uint64_t seed );
    siphash_64( void const* p, std::size_t n );
    constexpr siphash_64( unsigned char const* p, std::size_t n );

    void update( void const* p, std::size_t n );
    constexpr void update( unsigned char const* p, std::size_t n );

    constexpr result_type result();
  };
}