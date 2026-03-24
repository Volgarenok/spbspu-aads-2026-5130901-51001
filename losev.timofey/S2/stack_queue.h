#ifndef STACK_QUEUE_H
#define STACK_QUEUE_H

#include <stdexcept>
#include "../common/list.h"

namespace losev {

template<typename T>
class Stack {
public:
  Stack() = default;
  bool empty() const noexcept { return true; }
};

template<typename T>
class Queue {
public:
  Queue() = default;
  bool empty() const noexcept { return true; }
};

}

#endif
