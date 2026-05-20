#ifndef KARPENKO_ROBIN_HOOD_HASH_HPP
#define KARPENKO_ROBIN_HOOD_HASH_HPP

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>
#include <string>

namespace karpenko
{

struct StdHash
{
    std::size_t operator()(const std::string& s) const
    {
        return std::hash<std::string>{}(s);
    }
};

template <typename Key, typename Value,
          typename Hash = StdHash,
          typename Equal = std::equal_to<Key>>
class RobinHoodHashTable
{
private:
    struct Entry
    {
        Key key;
        Value value;
        bool occupied;
        size_t dist;

        Entry() : occupied(false), dist(0) {}
        Entry(const Key& k, const Value& v) : key(k), value(v), occupied(true), dist(0) {}
        Entry(Entry&& other) noexcept
            : key(std::move(other.key)), value(std::move(other.value)),
              occupied(other.occupied), dist(other.dist)
        {
            other.occupied = false;
        }
        Entry& operator=(Entry&& other) noexcept
        {
            if (this != &other)
            {
                key = std::move(other.key);
                value = std::move(other.value);
                occupied = other.occupied;
                dist = other.dist;
                other.occupied = false;
            }
            return *this;
        }
        Entry(const Entry&) = delete;
        Entry& operator=(const Entry&) = delete;
    };

    Entry* table_;
    size_t capacity_;
    size_t size_;
    Hash hasher_;
    Equal equal_;

    size_t nextPowerOfTwo(size_t n)
    {
        size_t p = 1;
        while (p < n) p <<= 1;
        return p;
    }

    void rehash(size_t newCap)
    {
        RobinHoodHashTable newTable(newCap, hasher_, equal_);
        for (size_t i = 0; i < capacity_; ++i)
            if (table_[i].occupied)
                newTable.insert(std::move(table_[i].key), std::move(table_[i].value));
        swap(newTable);
    }

    void checkGrow()
    {
        if (static_cast<float>(size_) / capacity_ > 0.8f)
            rehash(capacity_ * 2);
    }

public:
    RobinHoodHashTable(size_t initialCapacity = 16, Hash h = Hash(), Equal e = Equal())
        : capacity_(nextPowerOfTwo(initialCapacity)), size_(0), hasher_(h), equal_(e)
    {
        table_ = new Entry[capacity_];
    }

    ~RobinHoodHashTable() { delete[] table_; }
};

}

#endif
