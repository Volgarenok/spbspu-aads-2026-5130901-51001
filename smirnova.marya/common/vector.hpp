#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>

namespace smirnova
{

  template <typename T>
  class Vector
  {
  private:
    T* data;
    size_t sz;
    size_t cap;

    void expansion(size_t newCap)
    {
      if (newCap <= cap)
        return;

      T* newData = new T[newCap];

      for (size_t i = 0; i < sz; ++i)
      {
        newData[i] = std::move(data[i]);
      }

      delete[] data;
      data = newData;
      cap = newCap;
    }

  public:
    Vector():
      data(nullptr), sz(0), cap(0)
    {}

    Vector(const Vector& other):
      data(nullptr), sz(other.sz), cap(other.cap)
    {
      if (cap > 0)
        data = new T[cap];

      for (size_t i = 0; i < sz; ++i)
        data[i] = other.data[i];
    }

    Vector& operator=(const Vector& other)
    {
      if (this != &other)
      {
        Vector tmp(other);
        std::swap(data, tmp.data);
        std::swap(sz, tmp.sz);
        std::swap(cap, tmp.cap);
      }
      return *this;
    }

    ~Vector()
    {
      delete[] data;
    }

    size_t size() const
    {
      return sz;
    }

    T& operator[](size_t i)
    {
      return data[i];
    }
    
    const T& operator[](size_t i) const
    {
      return data[i];
    }

    void pushBack(const T& v)
    {
      if (sz == cap)
      {
        expansion(cap == 0 ? 2 : cap * 2);
      }
      data[sz++] = v;
    }

    void pushBack(T&& v)
    {
      if (sz == cap)
      {
        expansion(cap == 0 ? 2 : cap * 2);
      }
      data[sz++] = std::move(v);
    }

    class iterator
    {
      T* ptr;

    public:
      iterator(T* p = nullptr):
        ptr(p)
      {}

      T& operator*()
      {
        return *ptr;
      }

      iterator& operator++()
      {
        ++ptr;
        return *this;
      }

      bool operator!=(const iterator& other) const
      {
        return ptr != other.ptr;
      }
    };

    iterator begin()
    {
      return iterator(data);
    }
    iterator end()
    {
      return iterator(data + sz);
    }
  };

}

#endif