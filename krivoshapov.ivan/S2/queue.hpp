#ifndef KRIVOSHAPOV_QUEUE_HPP
#define KRIVOSHAPOV_QUEUE_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace krivoshapov
{
  template <typename T>
  class Queue
  {
  public:
    Queue() : data_(nullptr),
              head_(0),
              count_(0),
              cap_(0)
    {
    }

    ~Queue()
    {
      deallocate(data_);
    }

  private:
    T *data_;
    size_t head_;
    size_t count_;
    size_t cap_;

    static T *allocate(size_t n) { return new T[n](); }
    static void deallocate(T *p) noexcept { delete[] p; }
  };
}

#endif
