#ifndef LIST_IMPL_HPP
#define LIST_IMPL_HPP

#include <stdexcept>

namespace loseva {

template <typename T>
List<T>::List() : head_(nullptr), tail_(nullptr), size_(0) {}

template <typename T>
List<T>::~List() {
    clear();
}

template <typename T>
List<T>::List(const List& other) : List() {
    for (auto it = other.begin(); it != other.end(); ++it)
        push_back(*it);
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this != &other) {
        clear();
        for (auto it = other.begin(); it != other.end(); ++it)
            push_back(*it);
    }
    return *this;
}

template <typename T>
List<T>::List(List&& other) noexcept
    : head_(other.head_), tail_(other.tail_), size_(other.size_) {
    other.head_ = other.tail_ = nullptr;
    other.size_ = 0;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    if (this != &other) {
        clear();
        head_ = other.head_;
        tail_ = other.tail_;
        size_ = other.size_;

        other.head_ = other.tail_ = nullptr;
        other.size_ = 0;
    }
    return *this;
}

template <typename T>
bool List<T>::empty() const {
    return size_ == 0;
}

template <typename T>
size_t List<T>::size() const {
    return size_;
}

template <typename T>
void List<T>::clear() {
    while (!empty())
        pop_front();
}

template <typename T>
T& List<T>::front() {
    if (empty()) throw std::out_of_range("empty list");
    return head_->data;
}

template <typename T>
T& List<T>::back() {
    if (empty()) throw std::out_of_range("empty list");
    return tail_->data;
}

template <typename T>
void List<T>::push_back(const T& val) {
    Node* n = new Node(val);

    if (empty()) {
        head_ = tail_ = n;
    } else {
        tail_->next = n;
        n->prev = tail_;
        tail_ = n;
    }
    ++size_;
}

template <typename T>
void List<T>::push_front(const T& val) {
    Node* n = new Node(val);

    if (empty()) {
        head_ = tail_ = n;
    } else {
        n->next = head_;
        head_->prev = n;
        head_ = n;
    }
    ++size_;
}

template <typename T>
void List<T>::pop_back() {
    if (empty()) return;

    Node* tmp = tail_;
    tail_ = tail_->prev;

    if (tail_) tail_->next = nullptr;
    else head_ = nullptr;

    delete tmp;
    --size_;
}

template <typename T>
void List<T>::pop_front() {
    if (empty()) return;

    Node* tmp = head_;
    head_ = head_->next;

    if (head_) head_->prev = nullptr;
    else tail_ = nullptr;

    delete tmp;
    --size_;
}

template <typename T>
typename List<T>::iterator
List<T>::insert(iterator pos, const T& val) {
    if (pos.ptr_ == nullptr) {
        push_back(val);
        return iterator(tail_);
    }

    Node* cur = pos.ptr_;
    Node* n = new Node(val);

    n->next = cur;
    n->prev = cur->prev;

    if (cur->prev)
        cur->prev->next = n;
    else
        head_ = n;

    cur->prev = n;

    ++size_;
    return iterator(n);
}

template <typename T>
typename List<T>::iterator
List<T>::erase(iterator pos) {
    if (pos.ptr_ == nullptr) return end();

    Node* cur = pos.ptr_;
    Node* next = cur->next;

    if (cur->prev)
        cur->prev->next = cur->next;
    else
        head_ = cur->next;

    if (cur->next)
        cur->next->prev = cur->prev;
    else
        tail_ = cur->prev;

    delete cur;
    --size_;

    return iterator(next);
}

template <typename T>
typename List<T>::iterator List<T>::begin() {
    return iterator(head_);
}

template <typename T>
typename List<T>::iterator List<T>::end() {
    return iterator(nullptr);
}

template <typename T>
typename List<T>::const_iterator List<T>::begin() const {
    return const_iterator(head_);
}

template <typename T>
typename List<T>::const_iterator List<T>::end() const {
    return const_iterator(nullptr);
}

}

#endif
