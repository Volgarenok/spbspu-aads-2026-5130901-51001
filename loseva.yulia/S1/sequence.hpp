#ifndef SEQUENCE_HPP
#define SEQUENCE_HPP

#include <cstddef>

namespace loseva {

template <typename T>
class Sequence
{
public:
  virtual ~Sequence();

  virtual void pushBack(const T &value) = 0;
  virtual void pushFront(const T &value) = 0;

  virtual void popBack() = 0;
  virtual void popFront() = 0;

  virtual std::size_t size() const = 0;
  virtual bool empty() const = 0;

  virtual T &front() = 0;
  virtual const T &front() const = 0;

  virtual T &back() = 0;
  virtual const T &back() const = 0;
};

}

#endif
