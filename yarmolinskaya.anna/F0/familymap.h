#ifndef YARMOLINSKAYA_FAMILY_MAP_H
#define YARMOLINSKAYA_FAMILY_MAP_H

#include <string>
#include "NavTree.h"

namespace yarmolinskaya {
  class FamilyMap {
  private:
    struct Bucket {
      std::string key;
      NavTree value;
      bool occupied = false;
    };

    Bucket* m_table = nullptr;
    size_t m_capacity = 16;
    size_t m_sizeCount = 0;

    size_t getHash(const std::string& str) const
    {
      size_t hash = 5381;
      for (char c : str) {
        hash = ((hash << 5) + hash) + c;
      }
      return hash % m_capacity;
    }

    void resize()
    {
      size_t oldCap = m_capacity;
      Bucket* oldTable = m_table;
      m_capacity *= 2;
      m_table = new Bucket[m_capacity];
      m_sizeCount = 0;
      for (size_t i = 0; i < oldCap; ++i) {
        if (oldTable[i].occupied) {
          insert(oldTable[i].key, oldTable[i].value);
        }
      }
      delete[] oldTable;
    }

  public:
    FamilyMap()
    {
      m_table = new Bucket[m_capacity];
    }

    ~FamilyMap()
    {
      delete[] m_table;
    }

    void insert(const std::string& key, const NavTree& val)
    {
      if (m_sizeCount * 2 >= m_capacity) {
        resize();
      }
      size_t idx = getHash(key);
      while (m_table[idx].occupied) {
        if (m_table[idx].key == key) {
          m_table[idx].value = val;
          return;
        }
        idx = (idx + 1) % m_capacity;
      }
      m_table[idx].key = key;
      m_table[idx].value = val;
      m_table[idx].occupied = true;
      m_sizeCount++;
    }

    NavTree* find(const std::string& key)
    {
      size_t idx = getHash(key);
      size_t start = idx;
      while (m_table[idx].occupied) {
        if (m_table[idx].key == key) {
          return &m_table[idx].value;
        }
        idx = (idx + 1) % m_capacity;
        if (idx == start) {
          break;
        }
      }
      return nullptr;
    }
  };
}

#endif
