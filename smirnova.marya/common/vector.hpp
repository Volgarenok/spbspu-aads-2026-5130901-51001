#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include "ector-Iterator.hpp"

namespace smirnova
{

template <typename T>
class Vector
{
private:
  T* data;
  size_t sz;
  size_t cap;

  void expansion(size_t newCap);
  void ensureCapacity();

public:
  Vector()
    : data(0), sz(0), cap(0)
  {}

  ~Vector()
  {
    delete[] data;
  }

  size_t size() const;

  T& operator[](size_t i);
  const T& operator[](size_t i) const;

  void pushBack(const T& v);
  void pushBack(T&& v);

  VectorIterator<T> begin();
  VectorIterator<T> end();

  ConstVectorIterator<T> begin() const;
  ConstVectorIterator<T> end() const;
};

}

template <typename T>
size_t smirnova::Vector<T>::size() const
{
  return sz;
}

template <typename T>
T& smirnova::Vector<T>::operator[](size_t i)
{
  return data[i];
}

template <typename T>
const T& smirnova::Vector<T>::operator[](size_t i) const
{
  return data[i];
}

template <typename T>
void smirnova::Vector<T>::expansion(size_t newCap)
{
  if (newCap <= cap)
    return;

  T* newData = new T[newCap];

  for (size_t i = 0; i < sz; ++i)
    newData[i] = data[i];

  delete[] data;
  data = newData;
  cap = newCap;
}

template <typename T>
void smirnova::Vector<T>::ensureCapacity()
{
  if (sz < cap)
    return;

  size_t newCap = (cap == 0 ? 2 : cap * 2);
  expansion(newCap);
}

template <typename T>
void smirnova::Vector<T>::pushBack(const T& v)
{
  ensureCapacity();
  data[sz++] = v;
}

template <typename T>
void smirnova::Vector<T>::pushBack(T&& v)
{
  ensureCapacity();
  data[sz++] = v;
}

template <typename T>
smirnova::VectorIterator<T> smirnova::Vector<T>::begin()
{
  return VectorIterator<T>(data);
}

template <typename T>
smirnova::VectorIterator<T> smirnova::Vector<T>::end()
{
  return VectorIterator<T>(data + sz);
}

template <typename T>
smirnova::ConstVectorIterator<T> smirnova::Vector<T>::begin() const
{
  return ConstVectorIterator<T>(data);
}

template <typename T>
smirnova::ConstVectorIterator<T> smirnova::Vector<T>::end() const
{
  return ConstVectorIterator<T>(data + sz);
}

#endif

