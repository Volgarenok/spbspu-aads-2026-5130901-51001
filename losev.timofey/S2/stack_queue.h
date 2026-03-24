#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include <stdexcept>
#include "../common/list.h"

namespace losev {

template<typename T>
class Stack {
public:
  Stack() = default;
  Stack(const Stack& other);
  Stack(Stack&& other) noexcept;
  ~Stack() = default;
  
  Stack& operator=(const Stack& other);
  Stack& operator=(Stack&& other) noexcept;
  
  bool empty() const noexcept;
  void clear() noexcept;
  
private:
  List<T> data_;
};

template<typename T>
class Queue {
public:
  Queue() = default;
  bool empty() const noexcept { return true; }
};

}

#endif
