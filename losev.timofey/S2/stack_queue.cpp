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

template<typename T>
void Stack<T>::push(const T& value) {
  data_.push_front(value);
}

template<typename T>
void Stack<T>::push(T&& value) {
  data_.push_front(std::move(value));
}

template<typename T>
const T& Stack<T>::top() const {
  if (empty()) {
    throw std::runtime_error("Stack is empty");
  }
  return *data_.begin();
}

template<typename T>
T& Stack<T>::top() {
  if (empty()) {
    throw std::runtime_error("Stack is empty");
  }
  return *data_.begin();
}

template<typename T>
T Stack<T>::pop() {
  if (empty()) {
    throw std::runtime_error("Stack is empty");
  }
  T value = std::move(*data_.begin());
  data_.pop_front();
  return val
}

template class Stack<int>;
template class Queue<int>;

}
