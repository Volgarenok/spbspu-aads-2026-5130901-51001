#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP

#include <functional>
#include <stdexcept>
#include <utility>

#include "list.hpp"

namespace shevchenko
{
template< class Key, class Value >
struct HashPair
{
  Key first;
  Value second;
  
  HashPair():
  first(),
  second()
  {
  }
  
  HashPair(const Key& key, const Value& value):
  first(key),
  second(value)
  {
  }
};

template<class Key, class Value, class Hash, class Equal = std::equal_to< Key>>
class HashTable
{
public:
  using value_type = HashPair< Key, Value >;
  using bucket_type = List< value_type >;
  
  explicit HashTable( size_t bucket_count = , size_t bucket_size = ):
  bucket_count_(bucket_count),
  bucket_size_(bucket_size),
  size_(0),
  buckets_(new bucket_type[bucket_count])
  {
  }
  
  HashTable(const HashTable& other):
  bucket_count_(other.bucket_count_),
  bucket_size_(other.bucket_size_),
  size_(other.size_),
  buckets_(new bucket_type[other.bucket_count_])
  {
    for (size_t i = 0; i < bucket_count_; ++i)
    {
      buckets_[i] = other.buckets_[i];
    }
  }
  
  HashTable& operator=(const HashTable& other)
  {
    if (this != &other)
    {
      HashTable tmp(other);
      swap(tmp);
    }
    
    return *this;
  }
  
  ~HashTable()
  {
    delete[] buckets_;
  }
  
  bool empty() const
  {
    return size_ == 0;
  }
  
  size_t size() const
  {
    return size_;
  }
  
  bool contains(const Key& key) const
  {
    size_t index = getIndex(key);
    
    for (auto it = buckets_[index].begin();
         it != buckets_[index].end();
         ++it)
    {
      if (equal_((*it).first, key))
      {
        return true;
      }
    }
    
    return false;
  }
  
  void insert(const Key& key, const Value& value)
  {
    size_t index = getIndex(key);
    
    bucket_type& bucket = buckets_[index];
    
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        (*it).second = value;
        return;
      }
    }
    
    if (bucket.size() >= bucket_size_)
    {
      throw std::overflow_error("bucket overflow");
    }
    
    bucket.pushBack(value_type(key, value));
    ++size_;
  }
  
  Value& at(const Key& key)
  {
    size_t index = getIndex(key);
    
    bucket_type& bucket = buckets_[index];
    
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        return (*it).second;
      }
    }
    
    throw std::out_of_range("key not found");
  }
  
  const Value& at(const Key& key) const
  {
    size_t index = getIndex(key);
    
    const bucket_type& bucket = buckets_[index];
    
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        return (*it).second;
      }
    }
    
    throw std::out_of_range("key not found");
  }
  
  void erase(const Key& key)
  {
    size_t index = getIndex(key);
    bucket_type& bucket = buckets_[index];
    
    for (auto it = bucket.begin(); it != bucket.end(); ++it)
    {
      if (equal_((*it).first, key))
      {
        bucket.erase(it);
        --size_;
        return;
      }
    }
    
    throw std::out_of_range("key not found");
  }
  
  void rehash(size_t new_bucket_count)
  {
    HashTable tmp(new_bucket_count, bucket_size_);
    
    for (size_t i = 0; i < bucket_count_; ++i)
    {
      for (auto it = buckets_[i].begin();
           it != buckets_[i].end();
           ++it)
      {
        tmp.insert((*it).first, (*it).second);
      }
    }
    
    swap(tmp);
  }
  
  void swap(HashTable& other) noexcept
  {
    std::swap(bucket_count_, other.bucket_count_);
    std::swap(bucket_size_, other.bucket_size_);
    std::swap(size_, other.size_);
    std::swap(buckets_, other.buckets_);
  }
  
private:
  size_t getIndex(const Key& key) const
  {
    return hash_(key) % bucket_count_;
  }
  
  size_t bucket_count_;
  size_t bucket_size_;
  size_t size_;
  
  bucket_type* buckets_;
  
  Hash hash_;
  Equal equal_;
};
}

#endif
