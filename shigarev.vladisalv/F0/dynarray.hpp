#ifndef YAFU_DYNARRAY_HPP
#define YAFU_DYNARRAY_HPP

#include <cstddef>

namespace yafu {
  namespace detail {
    constexpr size_t initialArrayCapacity = 8;
    constexpr size_t arrayGrowthFactor = 2;
  }

  template< typename T >
  class DynArray
  {
  public:
    DynArray():
      data_(nullptr),
      size_(0),
      capacity_(0)
    {}

    DynArray(const DynArray< T >& other):
      data_(nullptr),
      size_(0),
      capacity_(0)
    {
      reserve(other.size_);
      for (size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
      }
      size_ = other.size_;
    }

    ~DynArray()
    {
      delete[] data_;
    }

    DynArray< T >& operator=(const DynArray< T >& other)
    {
      if (this == &other) {
        return *this;
      }

      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;

      reserve(other.size_);
      for (size_t i = 0; i < other.size_; ++i) {
        data_[i] = other.data_[i];
      }
      size_ = other.size_;
      return *this;
    }

    T& operator[](size_t index)
    {
      return data_[index];
    }

    const T& operator[](size_t index) const
    {
      return data_[index];
    }

    size_t getSize() const
    {
      return size_;
    }

    bool isEmpty() const
    {
      return size_ == 0;
    }

    void pushBack(const T& value)
    {
      if ((size_ + 1) > capacity_) {
        const size_t grown = (capacity_ == 0)
          ? detail::initialArrayCapacity
          : capacity_ * detail::arrayGrowthFactor;
        reserve(grown);
      }
      data_[size_] = value;
      ++size_;
    }

    void reserve(size_t newCapacity)
    {
      if (newCapacity <= capacity_) {
        return;
      }

      T* buffer = new T[newCapacity];
      for (size_t i = 0; i < size_; ++i) {
        buffer[i] = data_[i];
      }

      delete[] data_;
      data_ = buffer;
      capacity_ = newCapacity;
    }

  private:
    T* data_;
    size_t size_;
    size_t capacity_;
  };
}

#endif
