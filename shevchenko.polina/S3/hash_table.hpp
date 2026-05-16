#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <cstddef>
#include <utility>
#include <functional>
#include <stdexcept>

namespace shevchenko {

template<typename Key, typename Value>
struct BucketNode {
  Key key;
  Value value;
  BucketNode* next;
  
  BucketNode(const Key& k, const Value& v, BucketNode* n = nullptr)
  : key(k)
  , value(v)
  , next(n) {
  }
};

template<typename Key, typename Value, typename Hash, typename Equal>
class HashTable;

template<typename Key, typename Value, typename Hash, typename Equal>
class HashTableIterator {
private:
  friend class HashTable<Key, Value, Hash, Equal>;
  
  HashTableIterator(typename HashTable<Key, Value, Hash, Equal>::node_type** buckets, size_t bucket_count, size_t bucket_index, typename HashTable<Key, Value, Hash, Equal>::node_type* node)
  : buckets_(buckets)
  , bucket_count_(bucket_count)
  , bucket_index_(bucket_index)
  , node_(node) {
  }

  typename HashTable<Key, Value, Hash, Equal>::node_type** buckets_;
  size_t bucket_count_;
  size_t bucket_index_;
  typename HashTable<Key, Value, Hash, Equal>::node_type* node_;
  
public:
  
};

template<typename Key, typename Value,
typename Hash = std::hash<Key>,
typename Equal = std::equal_to<Key>>
class HashTable {
public:
  using key_type = Key;
  using mapped_type = Value;
  using hasher = Hash;
  using key_equal = Equal;
  using node_type = BucketNode<Key, Value>;
  using iterator = HashTableIterator<Key, Value, Hash, Equal>;

  explicit HashTable(size_t initial_buckets = 16)
  : bucket_count_(initial_buckets > 0 ? initial_buckets : 16)
  , size_(0)
  , buckets_(new node_type*[bucket_count_]())
  , hash_()
  , equal_() {
    for (size_t i = 0; i < bucket_count_; ++i) {
      buckets_[i] = nullptr;
    }
  }

  ~HashTable() {
    clear();
    delete[] buckets_;
  }

  HashTable(const HashTable&) = delete;
  HashTable& operator=(const HashTable&) = delete;

  HashTable(HashTable&& other) noexcept
  : bucket_count_(other.bucket_count_)
  , size_(other.size_)
  , buckets_(other.buckets_)
  , hash_(std::move(other.hash_))
  , equal_(std::move(other.equal_)) {
    other.buckets_ = nullptr;
    other.bucket_count_ = 0;
    other.size_ = 0;
  }
  
  HashTable& operator=(HashTable&& other) noexcept {
    if (this != &other) {
      clear();
      delete[] buckets_;
      bucket_count_ = other.bucket_count_;
      size_ = other.size_;
      buckets_ = other.buckets_;
      hash_ = std::move(other.hash_);
      equal_ = std::move(other.equal_);
      other.buckets_ = nullptr;
      other.bucket_count_ = 0;
      other.size_ = 0;
    }
    return *this;
  }

  void clear() noexcept {
    for (size_t i = 0; i < bucket_count_; ++i) {
      node_type* current = buckets_[i];
      while (current != nullptr) {
        node_type* next = current->next;
        delete current;
        current = next;
      }
      buckets_[i] = nullptr;
    }
    size_ = 0;
  }

  void add(const Key& key, const Value& value) {

    (void)key;
    (void)value;
  }
  
  bool has(const Key& key) const {

    (void)key;
    return false;
  }
  
  size_t size() const noexcept {
    return size_;
  }
  
  bool empty() const noexcept {
    return size_ == 0;
  }
  
  size_t bucket_count() const noexcept {
    return bucket_count_;
  }
  
private:
  size_t bucket_count_;
  size_t size_;
  node_type** buckets_;
  Hash hash_;
  Equal equal_;
};

}

#endif
