#include "stack_queue.h"

namespace losev {

template<typename T>
Stack<T>::Stack(const Stack& other) : data_(other.data_) {}

template<typename T>
Stack<T>::Stack(Stack&& other) noexcept : data_(std::move(other.data_)) {}

template<typename T>
Stack<T>& Stack<T>::operator=(const Stack& other) {
  if (this != &other) {
    data_ = other.data_;
  }
  return *this;
}

template<typename T>
Stack<T>& Stack<T>::operator=(Stack&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

template<typename T>
bool Stack<T>::empty() const noexcept {
  return data_.empty();
}

template<typename T>
void Stack<T>::clear() noexcept {
  data_.clear();
}

template class Stack<int>;
template class Queue<int>;

}
