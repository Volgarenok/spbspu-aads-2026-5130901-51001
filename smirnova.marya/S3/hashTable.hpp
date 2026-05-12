#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include "../common/list.hpp"
#include "../common/vector.hpp"
#include "blake2.hpp"

namespace smirnova
{

template<class K, class V,
         class Hash = Blake2Hasher<K>,
         class Equal = std::equal_to<K>>
class HashTable
{
public:
  struct Pair
  {
    K key;
    V value;
  };

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
};

}

#endif

