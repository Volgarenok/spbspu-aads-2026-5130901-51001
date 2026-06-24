#ifndef YARMOLINSKAYA_NAV_TREE_H
#define YARMOLINSKAYA_NAV_TREE_H

#include "person.h"
#include "vector.h"

namespace yarmolinskaya {
  struct NavNode {
  public:
    Person* m_personData = nullptr;
    size_t m_hashKey = 0;
    int m_distance = 0;
    NavNode* m_left = nullptr;
    NavNode* m_right = nullptr;

    NavNode(Person* p, size_t key, int dist)
    {
      m_personData = p;
      m_hashKey = key;
      m_distance = dist;
    }
  };

  class NavTree {
  public:
    NavTree() = default;
    ~NavTree()
    {
      clear(m_root);
    }
    NavTree(const NavTree& other)
    {
      if (other.m_root) {
        Vector<Person*> list = other.getAllPersons();
        for (size_t i = 0; i < list.size(); ++i) {
          insert(list[i]);
        }
      }
    }
    NavTree& operator=(const NavTree& other)
    {
      if (this != &other) {
        clear(m_root);
        m_root = nullptr;
        if (other.m_root) {
          Vector<Person*> list = other.getAllPersons();
          for (size_t i = 0; i < list.size(); ++i) {
            insert(list[i]);
          }
        }
      }
      return *this;
    }
    void insert(Person* person)
    {
      if (!person) {
        return;
      }
      size_t key = calculateHash(person->m_name);
      if (!m_root) {
        m_root = new NavNode(person, key, 0);
        return;
      }
      NavNode* current = m_root;
      size_t currentKey = key;
      Person* currentPerson = person;
      int currentDist = 0;
      while (true) {
        if (currentKey == current->m_hashKey) {
          currentKey++;
        }
        if (currentDist > current->m_distance) {
          std::swap(currentPerson, current->m_personData);
          std::swap(currentKey, current->m_hashKey);
          std::swap(currentDist, current->m_distance);
        }
        if (currentKey < current->m_hashKey) {
          if (!current->m_left) {
            current->m_left = new NavNode(currentPerson, currentKey, currentDist + 1);
            break;
          }
          current = current->m_left;
        } else {
          if (!current->m_right) {
            current->m_right = new NavNode(currentPerson, currentKey, currentDist + 1);
            break;
          }
          current = current->m_right;
        }
        currentDist++;
      }
    }
    Person* search(const std::string& name) const
    {
      size_t key = calculateHash(name);
      NavNode* current = m_root;
      while (current) {
        if (current->m_personData->m_name == name) {
          return current->m_personData;
        }
        if (key < current->m_hashKey) {
          current = current->m_left;
        } else {
          current = current->m_right;
        }
      }
      return nullptr;
    }
    Vector<Person*> getAllPersons() const
    {
      Vector<Person*> list;
      collectAll(m_root, list);
      return list;
    }
  private:
    NavNode* m_root = nullptr;

    size_t calculateHash(const std::string& str) const
    {
      size_t hash = 5381;
      for (char c : str) {
        hash = ((hash << 5) + hash) + c;
      }
      return hash;
    }
    void collectAll(NavNode* node, Vector<Person*>& outList) const
    {
      if (!node) {
        return;
      }
      collectAll(node->m_left, outList);
      outList.push_back(node->m_personData);
      collectAll(node->m_right, outList);
    }
    void clear(NavNode* node)
    {
      if (!node) {
        return;
      }
      clear(node->m_left);
      clear(node->m_right);
      delete node;
    }
  };
}

#endif
