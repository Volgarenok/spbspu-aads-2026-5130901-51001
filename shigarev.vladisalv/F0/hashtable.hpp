#ifndef YAFU_HASHTABLE_HPP
#define YAFU_HASHTABLE_HPP

#include <cstddef>

#include "string.hpp"

namespace yafu {
  namespace detail {
    constexpr size_t defaultTableCapacity = 1009;
    constexpr size_t minimalTableCapacity = 11;
    constexpr size_t tableGrowthFactor = 2;
    constexpr size_t maxLoadNumerator = 7;
    constexpr size_t maxLoadDenominator = 10;
    constexpr size_t firstOddPrime = 3;
    constexpr unsigned long long firstHashBase = 31;
    constexpr unsigned long long secondHashBase = 37;

    bool isPrime(size_t value);
    size_t getNextPrime(size_t value);
    unsigned long long getPolynomialHash(const String& key, unsigned long long base);
  }

  template< typename V >
  class HashTable
  {
  public:
    explicit HashTable(size_t capacity = detail::defaultTableCapacity):
      slots_(nullptr),
      capacity_(detail::getNextPrime(capacity)),
      size_(0),
      usedSlots_(0)
    {
      slots_ = new Slot[capacity_];
    }

    HashTable(const HashTable< V >& other) = delete;

    ~HashTable()
    {
      delete[] slots_;
    }

    HashTable< V >& operator=(const HashTable< V >& other) = delete;

    void insert(const String& key, const V& value)
    {
      if (((usedSlots_ + 1) * detail::maxLoadDenominator)
          >= (capacity_ * detail::maxLoadNumerator)) {
        rehash(capacity_ * detail::tableGrowthFactor);
      }

      const size_t start = getFirstHash(key);
      const size_t step = getSecondHash(key);
      long firstDeleted = -1;

      for (size_t probe = 0; probe < capacity_; ++probe) {
        const size_t index = (start + probe * step) % capacity_;
        if (slots_[index].status == SlotOccupied) {
          if (slots_[index].key == key) {
            slots_[index].value = value;
            return;
          }
        } else if (slots_[index].status == SlotDeleted) {
          if (firstDeleted < 0) {
            firstDeleted = static_cast< long >(index);
          }
        } else {
          placeValue(key, value, index, firstDeleted);
          return;
        }
      }

      if (firstDeleted >= 0) {
        const size_t target = static_cast< size_t >(firstDeleted);
        slots_[target].key = key;
        slots_[target].value = value;
        slots_[target].status = SlotOccupied;
        ++size_;
        return;
      }

      rehash(capacity_ * detail::tableGrowthFactor);
      insert(key, value);
    }

    V* find(const String& key)
    {
      const size_t start = getFirstHash(key);
      const size_t step = getSecondHash(key);

      for (size_t probe = 0; probe < capacity_; ++probe) {
        const size_t index = (start + probe * step) % capacity_;
        if (slots_[index].status == SlotEmpty) {
          return nullptr;
        }
        if ((slots_[index].status == SlotOccupied) && (slots_[index].key == key)) {
          return &slots_[index].value;
        }
      }
      return nullptr;
    }

    bool remove(const String& key)
    {
      const size_t start = getFirstHash(key);
      const size_t step = getSecondHash(key);

      for (size_t probe = 0; probe < capacity_; ++probe) {
        const size_t index = (start + probe * step) % capacity_;
        if (slots_[index].status == SlotEmpty) {
          return false;
        }
        if ((slots_[index].status == SlotOccupied) && (slots_[index].key == key)) {
          slots_[index].status = SlotDeleted;
          slots_[index].key.clear();
          --size_;
          return true;
        }
      }
      return false;
    }

    size_t getSize() const
    {
      return size_;
    }

    size_t getCapacity() const
    {
      return capacity_;
    }

    bool isOccupied(size_t index) const
    {
      return slots_[index].status == SlotOccupied;
    }

    const String& getKeyAt(size_t index) const
    {
      return slots_[index].key;
    }

    const V& getValueAt(size_t index) const
    {
      return slots_[index].value;
    }

  private:
    enum SlotStatus
    {
      SlotEmpty,
      SlotOccupied,
      SlotDeleted
    };

    struct Slot
    {
      String key;
      V value;
      SlotStatus status = SlotEmpty;
    };

    Slot* slots_;
    size_t capacity_;
    size_t size_;
    size_t usedSlots_;

    size_t getFirstHash(const String& key) const
    {
      const unsigned long long hash = detail::getPolynomialHash(key, detail::firstHashBase);
      return static_cast< size_t >(hash % static_cast< unsigned long long >(capacity_));
    }

    size_t getSecondHash(const String& key) const
    {
      const unsigned long long hash = detail::getPolynomialHash(key, detail::secondHashBase);
      const unsigned long long divisor = static_cast< unsigned long long >(capacity_ - 1);
      return static_cast< size_t >(1 + hash % divisor);
    }

    void placeValue(const String& key, const V& value, size_t emptyIndex, long firstDeleted)
    {
      if (firstDeleted >= 0) {
        const size_t target = static_cast< size_t >(firstDeleted);
        slots_[target].key = key;
        slots_[target].value = value;
        slots_[target].status = SlotOccupied;
      } else {
        slots_[emptyIndex].key = key;
        slots_[emptyIndex].value = value;
        slots_[emptyIndex].status = SlotOccupied;
        ++usedSlots_;
      }
      ++size_;
    }

    void insertWithoutCheck(const String& key, const V& value)
    {
      const size_t start = getFirstHash(key);
      const size_t step = getSecondHash(key);

      for (size_t probe = 0; probe < capacity_; ++probe) {
        const size_t index = (start + probe * step) % capacity_;
        if (slots_[index].status != SlotOccupied) {
          slots_[index].key = key;
          slots_[index].value = value;
          slots_[index].status = SlotOccupied;
          ++size_;
          ++usedSlots_;
          return;
        }
      }
    }

    void rehash(size_t desiredCapacity)
    {
      const size_t newCapacity = detail::getNextPrime(desiredCapacity);
      Slot* oldSlots = slots_;
      const size_t oldCapacity = capacity_;

      slots_ = new Slot[newCapacity];
      capacity_ = newCapacity;
      size_ = 0;
      usedSlots_ = 0;

      for (size_t i = 0; i < oldCapacity; ++i) {
        if (oldSlots[i].status == SlotOccupied) {
          insertWithoutCheck(oldSlots[i].key, oldSlots[i].value);
        }
      }

      delete[] oldSlots;
    }
  };
}

#endif
