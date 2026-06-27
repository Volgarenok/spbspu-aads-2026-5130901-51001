#ifndef VECTOR_ITER_HPP
#define VECTOR_ITER_HPP

namespace smirnova
{
  template < class T >
  struct VectorIterator
  {
    T* ptr;

    VectorIterator(T* p = 0);

    T& operator*();
    T* operator->();
    VectorIterator& operator++();
    bool operator!=(const VectorIterator& other) const;
  };

  template < class T >
  struct ConstVectorIterator
  {
    const T* ptr;

    ConstVectorIterator(const T* p = 0);

    const T& operator*() const;
    const T* operator->() const;
    ConstVectorIterator& operator++();
    bool operator!=(const ConstVectorIterator& other) const;
  };

}

template < class T >
smirnova::VectorIterator< T >::VectorIterator(T* p):
  ptr(p)
{}

template < class T >
T& smirnova::VectorIterator< T >::operator*()
{
  return *ptr;
}

template < class T >
T* smirnova::VectorIterator< T >::operator->()
{
  return ptr;
}

template < class T >
smirnova::VectorIterator< T >& smirnova::VectorIterator< T >::operator++()
{
  ++ptr;
  return *this;
}

template < class T >
bool smirnova::VectorIterator<T>::operator!=(const VectorIterator& other) const
{
  return ptr != other.ptr;
}

template < class T >
smirnova::ConstVectorIterator< T >::ConstVectorIterator(const T* p):
  ptr(p)
{}

template < class T >
const T& smirnova::ConstVectorIterator< T >::operator*() const
{
  return *ptr;
}

template < class T >
const T* smirnova::ConstVectorIterator< T >::operator->() const
{
  return ptr;
}

template < class T >
smirnova::ConstVectorIterator< T >& smirnova::ConstVectorIterator< T >::operator++()
{
  ++ptr;
  return *this;
}

template < class T >
bool smirnova::ConstVectorIterator< T >::operator!=(const ConstVectorIterator& other) const
{
  return ptr != other.ptr;
}

#endif

