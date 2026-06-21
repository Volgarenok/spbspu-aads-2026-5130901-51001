#ifndef KRIVOSHAPOV_ARRAY_HPP
#define KRIVOSHAPOV_ARRAY_HPP

#include <cstddef>
#include <utility>

namespace krivoshapov
{
  template <typename T>
  class Array
  {
  public:
    Array() : data_(nullptr),
              size_(0),
              capacity_(0)
    {
    }

    Array(const Array<T> &other) : data_(nullptr),
                                   size_(0),
                                   capacity_(0)
    {
      reserve(other.size_);
      for (std::size_t i = 0; i < other.size_; ++i)
      {
        data_[i] = other.data_[i];
      }
      size_ = other.size_;
    }

    Array(Array<T> &&other) noexcept : data_(other.data_),
                                       size_(other.size_),
                                       capacity_(other.capacity_)
    {
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    ~Array()
    {
      delete[] data_;
    }

    Array<T> &operator=(const Array<T> &other)
    {
      if (this != &other)
      {
        Array<T> copy(other);
        swap(copy);
      }
      return *this;
    }

    Array<T> &operator=(Array<T> &&other) noexcept
    {
      if (this != &other)
      {
        delete[] data_;
        data_ = other.data_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
      }
      return *this;
    }

    T &operator[](std::size_t index)
    {
      return data_[index];
    }

    const T &operator[](std::size_t index) const
    {
      return data_[index];
    }

    void push(const T &value)
    {
      if (size_ == capacity_)
      {
        reserve(capacity_ == 0 ? initialCapacity : capacity_ * 2);
      }
      data_[size_] = value;
      ++size_;
    }

    std::size_t getSize() const
    {
      return size_;
    }

    bool isEmpty() const
    {
      return size_ == 0;
    }

  private:
    static const std::size_t initialCapacity = 4;

    T *data_;
    std::size_t size_;
    std::size_t capacity_;

    void reserve(std::size_t newCapacity)
    {
      if (newCapacity <= capacity_)
      {
        return;
      }
      T *const newData = new T[newCapacity];
      for (std::size_t i = 0; i < size_; ++i)
      {
        newData[i] = std::move(data_[i]);
      }
      delete[] data_;
      data_ = newData;
      capacity_ = newCapacity;
    }

    void swap(Array<T> &other) noexcept
    {
      std::swap(data_, other.data_);
      std::swap(size_, other.size_);
      std::swap(capacity_, other.capacity_);
    }
  };
}

#endif
