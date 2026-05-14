#ifndef LOSEV_HASH_TABLE_HPP
#define LOSEV_HASH_TABLE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

#include "../common/list.h"

namespace losev {

template<typename Key, typename Value>
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

  size_t bucketIndex(const Key& key) const
  {
    return 0;  // временно всегда в корзину 0
  }

public:
  explicit HashTable(size_t bucketCount)
    : buckets_(new Node*[bucketCount]())
    , bucketCount_(bucketCount)
    , size_(0)
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
      if (current->key == key) {
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
      if (current->key == key) {
        return current->value;
      }
      current = current->next;
    }
    throw std::out_of_range("key not found");
  }

  size_t size() const { return size_; }
  size_t bucketCount() const { return bucketCount_; }
  bool empty() const { return size_ == 0; }

  // Запрещаем копирование
  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  // Разрешаем перемещение
  HashTable(HashTable&& other) noexcept
    : buckets_(other.buckets_)
    , bucketCount_(other.bucketCount_)
    , size_(other.size_)
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
      other.buckets_ = nullptr;
      other.bucketCount_ = 0;
      other.size_ = 0;
    }
    return *this;
  }
};

}

#endif
