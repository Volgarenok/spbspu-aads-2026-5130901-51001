#ifndef VECTORITERATOR_HPP
#define VECTORITERATOR_HPP

namespace smirnova
{

template <typename T>
struct VectorIterator
{
  T* ptr;

  VectorIterator(T* p = 0);

  T& operator*();
  VectorIterator& operator++();
  bool operator!=(const VectorIterator& other) const;
};

template <typename T>
struct ConstVectorIterator
{
  const T* ptr;

  ConstVectorIterator(const T* p = 0);

  const T& operator*() const;
  ConstVectorIterator& operator++();
  bool operator!=(const ConstVectorIterator& other) const;
};

}

// ================= IMPLEMENTATION =================

template <typename T>
smirnova::VectorIterator<T>::VectorIterator(T* p)
  : ptr(p)
{}

template <typename T>
T& smirnova::VectorIterator<T>::operator*()
{
  return *ptr;
}

template <typename T>
smirnova::VectorIterator<T>& smirnova::VectorIterator<T>::operator++()
{
  ++ptr;
  return *this;
}

template <typename T>
bool smirnova::VectorIterator<T>::operator!=(const VectorIterator& other) const
{
  return ptr != other.ptr;
}

template <typename T>
smirnova::ConstVectorIterator<T>::ConstVectorIterator(const T* p)
  : ptr(p)
{}

template <typename T>
const T& smirnova::ConstVectorIterator<T>::operator*() const
{
  return *ptr;
}

template <typename T>
smirnova::ConstVectorIterator<T>& smirnova::ConstVectorIterator<T>::operator++()
{
  ++ptr;
  return *this;
}

template <typename T>
bool smirnova::ConstVectorIterator<T>::operator!=(const ConstVectorIterator& other) const
{
  return ptr != other.ptr;
}

#endif

