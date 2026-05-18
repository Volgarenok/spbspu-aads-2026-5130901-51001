#ifndef KRIVOSHAPOV_DYNAMIC_ARRAY_HPP
#define KRIVOSHAPOV_DYNAMIC_ARRAY_HPP

#include <cstddef>
#include <utility>

namespace krivoshapov
{
  template <class T>
  class Array
  {
  public:
    Array() : data_(nullptr), size_(0), cap_(0) {}

    ~Array()
    {
      clear();
      ::operator delete(data_);
    }

    void push_back(const T &v)
    {
      ensure(size_ + 1);
      new (data_ + size_) T(v);
      ++size_;
    }

    size_t size() const noexcept { return size_; }
    bool empty() const noexcept { return size_ == 0; }
    T &operator[](size_t i) { return data_[i]; }
    const T &operator[](size_t i) const { return data_[i]; }

    void clear() noexcept
    {
      for (size_t i = 0; i < size_; ++i)
      {
        data_[i].~T();
      }
      size_ = 0;
    }

  private:
    T *data_;
    size_t size_;
    size_t cap_;

    void ensure(size_t n)
    {
      if (n <= cap_)
      {
        return;
      }
      size_t nc = cap_ == 0 ? 4 : cap_ * 2;
      if (nc < n)
      {
        nc = n;
      }
      T *nd = static_cast<T *>(::operator new(nc * sizeof(T)));
      for (size_t i = 0; i < size_; ++i)
      {
        new (nd + i) T(std::move(data_[i]));
        data_[i].~T();
      }
      ::operator delete(data_);
      data_ = nd;
      cap_ = nc;
    }
  };
}

#endif
