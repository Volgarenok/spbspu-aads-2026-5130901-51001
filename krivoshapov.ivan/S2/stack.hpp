#ifndef KRIVOSHAPOV_STACK_HPP
#define KRIVOSHAPOV_STACK_HPP

#include <cstddef>
#include <utility>
#include <stdexcept>

namespace krivoshapov
{
  template <typename T>
  class Stack
  {
  private:
    T *data_;
    size_t size_;
    size_t cap_;

    static T *allocate(size_t n) { return new T[n](); }
    static void deallocate(T *p) noexcept { delete[] p; }
  };
}

#endif
