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

    RobinHoodHashTable(const RobinHoodHashTable& other)
        : capacity_(other.capacity_), size_(other.size_), hasher_(other.hasher_), equal_(other.equal_)
    {
        table_ = new Entry[capacity_];
        for (size_t i = 0; i < capacity_; ++i)
        {
            if (other.table_[i].occupied)
            {
                table_[i].key = other.table_[i].key;
                table_[i].value = other.table_[i].value;
                table_[i].occupied = true;
                table_[i].dist = other.table_[i].dist;
            }
            else
            {
                table_[i].occupied = false;
            }
        }
    }

    RobinHoodHashTable& operator=(const RobinHoodHashTable& other)
    {
        if (this != &other)
        {
            RobinHoodHashTable tmp(other);
            swap(tmp);
        }
        return *this;
    }

    RobinHoodHashTable(RobinHoodHashTable&& other) noexcept
        : table_(other.table_), capacity_(other.capacity_), size_(other.size_),
          hasher_(std::move(other.hasher_)), equal_(std::move(other.equal_))
    {
        other.table_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    RobinHoodHashTable& operator=(RobinHoodHashTable&& other) noexcept
    {
        if (this != &other)
        {
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
        checkGrow();
        size_t idx = hasher_(key) & (capacity_ - 1);
        Entry newEntry(key, value);
        newEntry.dist = 0;

        while (true)
        {
            if (!table_[idx].occupied)
            {
                table_[idx] = std::move(newEntry);
                ++size_;
                return;
            }
            if (equal_(table_[idx].key, key))
            {
                table_[idx].value = value;
                return;
            }
            if (newEntry.dist > table_[idx].dist)
                std::swap(newEntry, table_[idx]);
            ++idx;
            ++newEntry.dist;
            idx &= (capacity_ - 1);
        }
    }

    void insert(Key&& key, Value&& value)
    {
        checkGrow();
        size_t idx = hasher_(key) & (capacity_ - 1);
        Entry newEntry(std::move(key), std::move(value));
        newEntry.dist = 0;

        while (true)
        {
            if (!table_[idx].occupied)
            {
                table_[idx] = std::move(newEntry);
                ++size_;
                return;
            }
            if (equal_(table_[idx].key, key))
            {
                table_[idx].value = std::move(value);
                return;
            }
            if (newEntry.dist > table_[idx].dist)
                std::swap(newEntry, table_[idx]);
            ++idx;
            ++newEntry.dist;
            idx &= (capacity_ - 1);
        }
    }

    bool contains(const Key& key) const
    {
        size_t idx = hasher_(key) & (capacity_ - 1);
        size_t dist = 0;
        while (table_[idx].occupied && table_[idx].dist >= dist)
        {
            if (equal_(table_[idx].key, key))
                return true;
            ++idx;
            ++dist;
            idx &= (capacity_ - 1);
        }
        return false;
    }

    Value& at(const Key& key)
    {
        size_t idx = hasher_(key) & (capacity_ - 1);
        size_t dist = 0;
        while (table_[idx].occupied && table_[idx].dist >= dist)
        {
            if (equal_(table_[idx].key, key))
                return table_[idx].value;
            ++idx;
            ++dist;
            idx &= (capacity_ - 1);
        }
        throw std::out_of_range("key not found");
    }

    const Value& at(const Key& key) const
    {
        size_t idx = hasher_(key) & (capacity_ - 1);
        size_t dist = 0;
        while (table_[idx].occupied && table_[idx].dist >= dist)
        {
            if (equal_(table_[idx].key, key))
                return table_[idx].value;
            ++idx;
            ++dist;
            idx &= (capacity_ - 1);
        }
        throw std::out_of_range("key not found");
    }

    bool erase(const Key& key)
    {
        size_t idx = hasher_(key) & (capacity_ - 1);
        size_t dist = 0;
        while (table_[idx].occupied && table_[idx].dist >= dist)
        {
            if (equal_(table_[idx].key, key))
            {
                table_[idx].occupied = false;
                --size_;
                return true;
            }
            ++idx;
            ++dist;
            idx &= (capacity_ - 1);
        }
        return false;
    }

    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    class iterator
    {
        Entry* ptr;
        Entry* end;
        void skip() { while (ptr != end && !ptr->occupied) ++ptr; }
    public:
        iterator(Entry* p, Entry* e) : ptr(p), end(e) { skip(); }
        std::pair<Key&, Value&> operator*() { return {ptr->key, ptr->value}; }
        std::pair<Key&, Value&> operator->() { return {ptr->key, ptr->value}; }
        iterator& operator++() { ++ptr; skip(); return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr; skip(); return tmp; }
        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return !(*this == other); }
    };

    class const_iterator
    {
        const Entry* ptr;
        const Entry* end;
        void skip() { while (ptr != end && !ptr->occupied) ++ptr; }
    public:
        const_iterator(const Entry* p, const Entry* e) : ptr(p), end(e) { skip(); }
        std::pair<const Key&, const Value&> operator*() const { return {ptr->key, ptr->value}; }
        std::pair<const Key&, const Value&> operator->() const { return {ptr->key, ptr->value}; }
        const_iterator& operator++() { ++ptr; skip(); return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++ptr; skip(); return tmp; }
        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator& other) const { return !(*this == other); }
    };

    iterator begin() { return iterator(table_, table_ + capacity_); }
    iterator end() { return iterator(table_ + capacity_, table_ + capacity_); }
    const_iterator begin() const { return const_iterator(table_, table_ + capacity_); }
    const_iterator end() const { return const_iterator(table_ + capacity_, table_ + capacity_); }
};

}

#endif
