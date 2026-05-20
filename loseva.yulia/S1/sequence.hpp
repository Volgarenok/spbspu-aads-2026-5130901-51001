#ifndef LOSEVA_SEQUENCE_HPP
#define LOSEVA_SEQUENCE_HPP

#include <cstddef>

namespace loseva
{
  template<class T>
  class Sequence
  {
  public:
    virtual ~Sequence() = default;

    virtual void push_front(const T& value) = 0;
    virtual void pop_front() = 0;

    virtual bool empty() const = 0;
  };
}

#endif
