#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>
#include "vector-iter.hpp"

namespace smirnova
{
  template < class T >
  class Vector
  {
  private:
    T* data;
    size_t sz;
    size_t cap;

    void expansion(size_t newCap);
    void ensureCapacity();

  public:
    Vector():
      data(0),
      sz(0),
      cap(0)
    {}

    Vector(const Vector& other):
      data(0),
      sz(other.sz),
      cap(other.cap)
    {
      if (cap)
      {
        data = new T[cap];
        for (size_t i = 0; i < sz; ++i)
        {
          data[i] = other.data[i];
        }
      }
    }

    Vector(Vector&& other) noexcept:
      data(other.data),
      sz(other.sz),
      cap(other.cap)
    {
      other.data = 0;
      other.sz = 0;
      other.cap = 0;
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

    Vector& operator=(Vector&& other) noexcept
    {
      if (this != &other)
      {
        delete[] data;
        data = other.data;
        sz = other.sz;
        cap = other.cap;
        other.data = 0;
        other.sz = 0;
        other.cap = 0;
      }
      return *this;
    }

    ~Vector()
    {
      delete[] data;
    }

    size_t size() const;

    T& operator[](size_t i);
    const T& operator[](size_t i) const;

    void pushBack(const T& v);
    void pushBack(T&& v);
    void popBack();

    VectorIterator< T > begin();
    VectorIterator< T > end();

    ConstVectorIterator< T > begin() const;
    ConstVectorIterator< T > end() const;
  };

}

template < class T >
size_t smirnova::Vector< T >::size() const
{
  return sz;
}

template < class T >
T& smirnova::Vector< T >::operator[](size_t i)
{
  return data[i];
}

template < class T >
const T& smirnova::Vector< T >::operator[](size_t i) const
{
  return data[i];
}

template < class T >
void smirnova::Vector< T >::expansion(size_t newCap)
{
  if (newCap <= cap)
  {
    return;
  }

  T* newData = new T[newCap];

  for (size_t i = 0; i < sz; ++i)
  {
    newData[i] = data[i];
  }

  delete[] data;
  data = newData;
  cap = newCap;
}

template < class T >
void smirnova::Vector< T >::ensureCapacity()
{
  if (sz < cap)
  {
    return;
  }

  size_t newCap = (cap == 0) ? 2 : cap * 2;

  expansion(newCap);
}

template < class T >
void smirnova::Vector< T >::pushBack(const T& v)
{
  ensureCapacity();
  data[sz++] = v;
}

template < class T >
void smirnova::Vector< T >::pushBack(T&& v)
{
  ensureCapacity();
  data[sz++] = std::move(v);
}

template < class T >
void smirnova::Vector< T >::popBack()
{
  if (sz == 0)
  {
    throw std::out_of_range("pop_back on empty vector");
  }
  --sz;
}

template < class T >
smirnova::VectorIterator< T > smirnova::Vector< T >::begin()
{
  return VectorIterator< T >(data);
}

template < class T >
smirnova::VectorIterator< T > smirnova::Vector< T >::end()
{
  return VectorIterator< T >(data + sz);
}

template < class T >
smirnova::ConstVectorIterator< T > smirnova::Vector< T >::begin() const
{
  return ConstVectorIterator< T >(data);
}

template < class T >
smirnova::ConstVectorIterator< T > smirnova::Vector< T >::end() const
{
  return ConstVectorIterator< T >(data + sz);
}

#endif

