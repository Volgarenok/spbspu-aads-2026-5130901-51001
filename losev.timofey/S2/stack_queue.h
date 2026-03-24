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

  void push(const T& value);
  void push(T&& value);
  T pop();
  const T& top() const;
  T& top();
  bool empty() const noexcept;
  void clear() noexcept;

private:
  List<T> data_;
};

template<typename T>
class Queue {
public:
  Queue() = default;
  Queue(const Queue& other);
  Queue(Queue&& other) noexcept;
  ~Queue() = default;


  Queue& operator=(const Queue& other);
  Queue& operator=(Queue&& other) noexcept;

  bool empty() const noexcept;
  void clear() noexcept;
  void push(const T& value);
  void push(T&& value);
  const T& front() const;
  T& front();
  T pop();

private:
  List<T> data_;
  void push_back(const T& value);
  void push_back(T&& value);
};

}

#endif
