#include "stack_queue.h"
#include <utility>

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
  return value;
}


template<typename T>
Queue<T>::Queue(const Queue& other) : data_(other.data_) {}

template<typename T>
Queue<T>::Queue(Queue&& other) noexcept : data_(std::move(other.data_)) {}

template<typename T>
Queue<T>& Queue<T>::operator=(const Queue& other) {
  if (this != &other) {
    data_ = other.data_;
  }
  return *this;
}

template<typename T>
Queue<T>& Queue<T>::operator=(Queue&& other) noexcept {
  if (this != &other) {
    data_ = std::move(other.data_);
  }
  return *this;
}

template<typename T>
bool Queue<T>::empty() const noexcept {
  return data_.empty();
}

template<typename T>
void Queue<T>::clear() noexcept {
  data_.clear();
}

template<typename T>
void Queue<T>::push_back(const T& value) {
  if (data_.empty()) {
    data_.push_front(value);
    return;
  }
  List<T> temp;
  for (typename List<T>::iterator it = data_.begin(); it != data_.end(); ++it) {
    temp.push_front(*it);
  }
  temp.push_front(value);
  data_.clear();
  for (typename List<T>::iterator it = temp.begin(); it != temp.end(); ++it) {
    data_.push_front(*it);
  }
}

template<typename T>
void Queue<T>::push_back(T&& value) {
  if (data_.empty()) {
    data_.push_front(std::move(value));
    return;
  }
  List<T> temp;
  for (typename List<T>::iterator it = data_.begin(); it != data_.end(); ++it) {
    temp.push_front(*it);
  }
  temp.push_front(std::move(value));
  data_.clear();
  for (typename List<T>::iterator it = temp.begin(); it != temp.end(); ++it) {
    data_.push_front(*it);
  }
}

template<typename T>
void Queue<T>::push(const T& value) {
  push_back(value);
}

template<typename T>
void Queue<T>::push(T&& value) {
  push_back(std::move(value));
}

template<typename T>
const T& Queue<T>::front() const {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  return *data_.begin();
}

template<typename T>
T& Queue<T>::front() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  return *data_.begin();
}

template<typename T>
T Queue<T>::pop() {
  if (empty()) {
    throw std::runtime_error("Queue is empty");
  }
  T value = std::move(*data_.begin());
  data_.pop_front();
  return value;
}

template class Stack<int>;
template class Queue<int>;

template class Stack<char>;
template class Queue<char>;

}
