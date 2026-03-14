#ifndef LIST_HPP
#define LIST_HPP
#include <list>
#include <utility>
#include "iter.hpp"
#include "item.hpp"

namespace volkovich
{
  template <class T>
  class List
  {
  private:
    Item<T> fake_node;
    Item<T> *tail = nullptr;
    Item<T> *head = nullptr;
    size_t list_len;

  public:
    List() : fake_node(),
             list_len(0)
    {
      fake_node.next = nullptr;
    };

    ~List()
    {
      clear();
    };

    List(const List &other) : fake_node()
    {
      copy(other);
    };

    List(List &&other) : fake_node()
    {
      swap(other);
    };

    List &operator=(List other)
    {
      swap(other);
      return *this;
    };

    bool isEmpty() const noexcept
    {
      return fake_node.next == nullptr;
    };

    void popFront()
    {
      Item<T> *old_head = head;
      fake_node.next = old_head->next;
      head = fake_node.next;
      if (head == nullptr)
      {
        tail = nullptr;
      }
      delete old_head;
      list_len--;
    };

    void popBack()
    {
      if (fake_node.next == nullptr)
      {
        return;
      }
      Item<T> *old_tail = tail;
      Item<T> *prev = &fake_node;
      Item<T> *cur = fake_node.next;
      while (cur->next)
      {
        prev = cur;
        cur = prev->next;
      }
      prev->next = nullptr;
      delete cur;
      tail = prev;
      list_len--;
    };

    void pushFront(T &&v)
    {
      Item<T> *newItem = new Item<T>(std::move(v));
      newItem->next = head;
      fake_node.next = newItem;
      head = newItem;
      if (list_len == 0)
      {
        tail = newItem;
      }
      list_len++;
    };

    void pushFront(const T &v)
    {
      Item<T> *newItem = new Item<T>(v);
      newItem->next = head;
      fake_node.next = newItem;
      head = newItem;
      if (list_len == 0)
      {
        tail = newItem;
      }
      list_len++;
    };

    void pushBack(T &&v)
    {
      Item<T> *newItem = new Item<T>(std::move(v));
      if (list_len == 0)
      {
        tail = newItem;
        head = newItem;
        fake_node.next = head;
      }
      else
      {
        tail->next = newItem;
        tail = newItem;
      }
      list_len++;
    };

    void pushBack(const T &v)
    {
      Item<T> *newItem = new Item<T>(v);
      if (list_len == 0)
      {
        tail = newItem;
        head = newItem;
        fake_node.next = head;
      }
      else
      {
        tail->next = newItem;
        tail = newItem;
      }
      list_len++;
    };

    void insertAfter()
    {
    }

    void deleteAfter()
    {
    }

    void clear() noexcept
    {
      while (!isEmpty())
      {
        popFront();
      }
    };
    LIter<T> begin() noexcept;
    LCIter<T> begin() const noexcept;
    void swap(List &other) noexcept;
    void copy(const List &other) {

    };

    size_t size() noexcept
    {
      return list_len;
    };
  };
}

#endif
