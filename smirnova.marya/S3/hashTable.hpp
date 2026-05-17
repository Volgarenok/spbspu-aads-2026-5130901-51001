#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "../common/list.hpp"
#include "../common/vector.hpp"
#include "blake2.hpp"
#include "hashTableIter.hpp"
#include <stdexcept>

namespace smirnova
{
template<class K, class V, class Hash = Blake2Hasher<K>, class Equal = std::equal_to<K>>

class HashTable
{
public:
  struct Pair
  {
    K key;
    V value;
  };

  using Iterator = HashTableIterator<K, V, Hash, Equal>;

private:
  Vector<List<Pair>> table;
  size_t n;
  Hash hash;
  Equal eq;

  size_t index(const K& k) const
  {
    return hash(k) % n;
  }

public:
  HashTable(size_t size = 16) : n(size)
  {
    for (size_t i = 0; i < n; ++i)
      table.pushBack(List<Pair>());
  }

  size_t bucketCount() const { return n; }

  List<Pair>& bucket(size_t i) { return table[i]; }

  bool has(const K& k)
  {
    List<Pair>& b = table[index(k)];
    LIter<Pair> it = b.begin();

    while (it.valid())
    {
      if (eq(it.value().key, k))
        return true;
      it.next();
    }
    return false;
  }

  V& get(const K& k)
  {
    List<Pair>& b = table[index(k)];
    LIter<Pair> it = b.begin();

    while (it.valid())
    {
      if (eq(it.value().key, k))
        return it.value().value;
      it.next();
    }
    throw std::runtime_error("not found");
  }

  void add(const K& k, const V& v)
  {
    List<Pair>& b = table[index(k)];
    LIter<Pair> it = b.begin();

    while (it.valid())
    {
      if (eq(it.value().key, k))
      {
        it.value().value = v;
        return;
      }
      it.next();
    }

    b.pushBack(Pair{k, v});
  }

  V drop(const K& k)
  {
    size_t idx = index(k);
    List<Pair>& b = table[idx];

    LIter<Pair> it = b.begin();

    while (it.valid())
    {
      if (eq(it.value().key, k))
      {
        V old = it.value().value;

        List<Pair> newList;

        LIter<Pair> jt = b.begin();
        while (jt.valid())
        {
          if (!eq(jt.value().key, k))
            newList.pushBack(jt.value());
          jt.next();
        }

        b = newList;
        return old;
      }
      it.next();
    }

    throw std::runtime_error("not found");
  }

  void rehash(size_t slots)
  {
    Vector<List<Pair>> old = table;

    table = Vector<List<Pair>>();
    n = slots;

    for (size_t i = 0; i < n; ++i)
      table.pushBack(List<Pair>());

    for (size_t i = 0; i < old.size(); ++i)
    {
      LIter<Pair> it = old[i].begin();

      while (it.valid())
      {
        add(it.value().key, it.value().value);
        it.next();
      }
    }
  }

  Iterator begin()
  {
    return Iterator(this, 0);
  }

  Iterator end()
  {
    return Iterator(this, n);
  }
};

}

#endif

