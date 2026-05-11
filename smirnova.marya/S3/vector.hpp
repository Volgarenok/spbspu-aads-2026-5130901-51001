#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>

namespace smirnova
{
  template <typename T>
  class Vector
  {
    T* data;
    size_t sz;
    size_t cap;

    void increase()
    {
      size_t ncap = cap == 0 ? 2 : cap * 2;
      T* ndata = new T[ncap];

      for (size_t i = 0; i < sz; ++i)
      {
        ndata[i] = data[i];
      }

      delete[] data;
      data = ndata;
      cap = ncap;
    }

  public:
    Vector():
      data(nullptr), sz(nullptr), cap(nullptr)
    {}

    void pushBack(const T& v)
    {
      if (sz == cap)
      {
        increase();
      }
      data[sz++] = v;
    }

    T& operator[](std::size_t i)
    {
      return data[i];
    }
    const T& operator[](std::size_t i) const
    {
      return data[i];
    }
    size_t size() const
    {
      return sz;
    }

    ~Vector()
    {
      delete[] data;
    }
  };
}

#endif

