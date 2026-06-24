#ifndef YARMOLINSKAYA_VECTOR_H
#define YARMOLINSKAYA_VECTOR_H

#include <cstddef>
#include <utility>

namespace yarmolinskaya {
  template <typename T>
  class Vector {
  public:
    Vector() = default;
    ~Vector()
    {
      delete[] m_data;
    }
    Vector(const Vector& other)
    {
      if (other.m_sz > 0) {
        resize(other.m_cap);
        m_sz = other.m_sz;
        for (size_t i = 0; i < m_sz; ++i) {
          m_data[i] = other.m_data[i];
        }
      }
    }
    Vector& operator=(const Vector& other)
    {
      if (this != &other) {
        delete[] m_data;
        m_data = nullptr;
        m_cap = other.m_cap;
        m_sz = other.m_sz;
        if (m_cap > 0) {
          m_data = new T[m_cap];
          for (size_t i = 0; i < m_sz; ++i) {
            m_data[i] = other.m_data[i];
          }
        }
      }
      return *this;
    }
    void push_back(const T& val)
    {
      if (m_sz >= m_cap) {
        resize(m_cap == 0 ? 4 : m_cap * 2);
      }
      m_data[m_sz++] = val;
    }
    size_t size() const
    {
      return m_sz;
    }
    bool empty() const
    {
      return m_sz == 0;
    }
    T& operator[](size_t idx)
    {
      return m_data[idx];
    }
    const T& operator[](size_t idx) const
    {
      return m_data[idx];
    }
    T* begin()
    {
      return m_data;
    }
    T* end()
    {
      return m_data + m_sz;
    }
    const T* begin() const
    {
      return m_data;
    }
    const T* end() const
    {
      return m_data + m_sz;
    }
  private:
    T* m_data = nullptr;
    size_t m_cap = 0;
    size_t m_sz = 0;
    void resize(size_t newCap)
    {
      T* newData = new T[newCap];
      for (size_t i = 0; i < m_sz; ++i) {
        newData[i] = std::move(m_data[i]);
      }
      delete[] m_data;
      m_data = newData;
      m_cap = newCap;
    }
  };
}

#endif
