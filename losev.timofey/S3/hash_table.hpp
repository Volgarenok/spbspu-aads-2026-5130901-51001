#ifndef LOSEV_HASH_TABLE_HPP
#define LOSEV_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include <functional>

#include "../common/list.h"

namespace losev {

template<typename Key, typename Value,
         typename Hash = std::hash<Key>,
         typename Equal = std::equal_to<Key>>
class HashTable
{
private:
  struct Node
  {
    Key key;
    Value value;
    Node* next;

    Node(const Key& k, const Value& v, Node* n = nullptr)
      : key(k), value(v), next(n)
    {}
  };

  Node** buckets_;
  size_t bucketCount_;
  size_t size_;
  Hash hash_;
  Equal equal_;

  size_t bucketIndex(const Key& key) const
  {
    return hash_(key) % bucketCount_;
  }

public:
  explicit HashTable(size_t bucketCount)
    : buckets_(new Node*[bucketCount]())
    , bucketCount_(bucketCount)
    , size_(0)
    , hash_()
    , equal_()
  {}

  ~HashTable()
  {
    for (size_t i = 0; i < bucketCount_; ++i) {
      Node* current = buckets_[i];
      while (current != nullptr) {
        Node* temp = current;
        current = current->next;
        delete temp;
      }
    }
    delete[] buckets_;
  }

  void add(const Key& key, const Value& value)
  {
    size_t idx = bucketIndex(key);
    buckets_[idx] = new Node(key, value, buckets_[idx]);
    ++size_;
  }

  bool has(const Key& key) const
  {
    size_t idx = bucketIndex(key);
    Node* current = buckets_[idx];
    while (current != nullptr) {
      if (equal_(current->key, key)) {
        return true;
      }
      current = current->next;
    }
    return false;
  }

  Value get(const Key& key) const
  {
    size_t idx = bucketIndex(key);
    Node* current = buckets_[idx];
    while (current != nullptr) {
      if (equal_(current->key, key)) {
        return current->value;
      }
      current = current->next;
    }
    throw std::out_of_range("key not found");
  }

  Value drop(const Key& key)
  {
    size_t idx = bucketIndex(key);
    Node* prev = nullptr;
    Node* current = buckets_[idx];
    while (current != nullptr) {
      if (equal_(current->key, key)) {
        Value val = current->value;
        if (prev == nullptr) {
          buckets_[idx] = current->next;
        } else {
          prev->next = current->next;
        }
        delete current;
        --size_;
        return val;
      }
      prev = current;
      current = current->next;
    }
    throw std::out_of_range("key not found");
  }

  size_t size() const { return size_; }
  size_t bucketCount() const { return bucketCount_; }
  bool empty() const { return size_ == 0; }

  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  HashTable(HashTable&& other) noexcept
    : buckets_(other.buckets_)
    , bucketCount_(other.bucketCount_)
    , size_(other.size_)
    , hash_(std::move(other.hash_))
    , equal_(std::move(other.equal_))
  {
    other.buckets_ = nullptr;
    other.bucketCount_ = 0;
    other.size_ = 0;
  }

  HashTable& operator=(HashTable&& other) noexcept
  {
    if (this != &other) {
      this->~HashTable();
      buckets_ = other.buckets_;
      bucketCount_ = other.bucketCount_;
      size_ = other.size_;
      hash_ = std::move(other.hash_);
      equal_ = std::move(other.equal_);
      other.buckets_ = nullptr;
      other.bucketCount_ = 0;
      other.size_ = 0;
    }
    return *this;
  }
};

}

#endif
