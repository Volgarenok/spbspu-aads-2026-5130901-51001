#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace loseva {

template< class T >
class Sequence {
public:
  using size_type = std::size_t;

  Sequence():
    data_(nullptr),
    size_(0),
    capacity_(0)
  {}

  Sequence(const Sequence & other):
    data_(nullptr),
    size_(0),
    capacity_(0)
  {
    reserve(other.size_);
    for (size_type i = 0; i < other.size_; ++i) {
      data_[i] = other.data_[i];
    }
    size_ = other.size_;
  }

  Sequence(Sequence && other) noexcept:
    data_(other.data_),
    size_(other.size_),
    capacity_(other.capacity_)
  {
    other.data_ = nullptr;
    other.size_ = 0;
    other.capacity_ = 0;
  }

  ~Sequence()
  {
    delete[] data_;
  }

  Sequence & operator=(const Sequence & other)
  {
    if (this != &other) {
      Sequence tmp(other);
      swap(tmp);
    }
    return *this;
  }

  Sequence & operator=(Sequence && other) noexcept
  {
    if (this != &other) {
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

  void push_back(const T & value)
  {
    if (size_ == capacity_) {
      const size_type newCap = (capacity_ == 0) ? 4 : capacity_ * 2;
      reserve(newCap);
    }
    data_[size_] = value;
    ++size_;
  }

  void reserve(size_type newCapacity)
  {
    if (newCapacity <= capacity_) {
      return;
    }
    T * newData = new T[newCapacity];
    for (size_type i = 0; i < size_; ++i) {
      newData[i] = std::move(data_[i]);
    }
    delete[] data_;
    data_ = newData;
    capacity_ = newCapacity;
  }

  void erase_at(size_type index)
  {
    if (index >= size_) {
      throw std::out_of_range("Sequence::erase_at: index out of range");
    }
    for (size_type i = index; i + 1 < size_; ++i) {
      data_[i] = std::move(data_[i + 1]);
    }
    --size_;
  }

  void clear()
  {
    size_ = 0;
  }

  T & operator[](size_type index)
  {
    return data_[index];
  }

  const T & operator[](size_type index) const
  {
    return data_[index];
  }

  size_type size() const
  {
    return size_;
  }

  size_type capacity() const
  {
    return capacity_;
  }

  bool empty() const
  {
    return size_ == 0;
  }

  T * begin()
  {
    return data_;
  }

  T * end()
  {
    return data_ + size_;
  }

  const T * begin() const
  {
    return data_;
  }

  const T * end() const
  {
    return data_ + size_;
  }

private:
  T * data_;
  size_type size_;
  size_type capacity_;

  void swap(Sequence & other) noexcept
  {
    T * tmpData = data_;
    data_ = other.data_;
    other.data_ = tmpData;

    size_type tmpSize = size_;
    size_ = other.size_;
    other.size_ = tmpSize;

    size_type tmpCap = capacity_;
    capacity_ = other.capacity_;
    other.capacity_ = tmpCap;
  }
};

}

#endif
