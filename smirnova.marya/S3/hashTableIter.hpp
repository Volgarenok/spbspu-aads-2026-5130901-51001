#ifndef HASHTABLEITERATOR_HPP
#define HASHTABLEITERATOR_HPP

#include "../common/list.hpp"
#include <cstddef>

namespace smirnova
{

template<class K, class V, class Hash, class Equal>
class HashTable;

template<class K, class V, class Hash, class Equal>
class HashTableIterator
{
public:
  using Pair = typename HashTable<K, V, Hash, Equal>::Pair;

private:
  HashTable<K, V, Hash, Equal>* table;
  size_t bucketIndex;
  LIter<Pair> it;

  void advance()
  {
    if (!table)
      return;

    while (bucketIndex < table->bucketCount() && !it.valid())
    {
      ++bucketIndex;
      if (bucketIndex < table->bucketCount())
        it = table->bucket(bucketIndex).begin();
    }
  }

public:
  HashTableIterator(HashTable<K, V, Hash, Equal>* t = nullptr, size_t i = 0)
    : table(t), bucketIndex(i), it()
  {
    if (table && bucketIndex < table->bucketCount())
      it = table->bucket(bucketIndex).begin();
    advance();
  }

  Pair& operator*()
  {
    return it.value();
  }

  const Pair& operator*() const
  {
    return it.value();
  }

  HashTableIterator& operator++()
  {
    if (table && bucketIndex < table->bucketCount() && it.valid())
    {
      it.next();
      advance();
    }
    return *this;
  }

  bool operator!=(const HashTableIterator& other) const
  {
    if (table != other.table || bucketIndex != other.bucketIndex)
      return true;

    if (!it.valid() && !other.it.valid())
      return false;

    if (it.valid() && other.it.valid())
      return &it.value() != &other.it.value();

    return true;
  }
};

}

#endif