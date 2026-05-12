#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <utility>

namespace loseva {

template <typename T>
class List {
private:
    struct Node {
        T data;
        Node* next;
        Node* prev;

        Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;

public:
    class iterator {
        Node* ptr_;
    public:
        iterator(Node* p = nullptr) : ptr_(p) {}

        T& operator*() { return ptr_->data; }
        T* operator->() { return &ptr_->data; }

        iterator& operator++() { ptr_ = ptr_->next; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++(*this); return tmp; }

        iterator& operator--() { ptr_ = ptr_->prev; return *this; }

        bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }

        friend class List;
    };

    class const_iterator {
        const Node* ptr_;
    public:
        const_iterator(const Node* p = nullptr) : ptr_(p) {}

        const T& operator*() const { return ptr_->data; }

        const_iterator& operator++() { ptr_ = ptr_->next; return *this; }

        bool operator!=(const const_iterator& other) const {
            return ptr_ != other.ptr_;
        }

        friend class List;
    };

    List();
    ~List();

    List(const List& other);
    List& operator=(const List& other);

    List(List&& other) noexcept;
    List& operator=(List&& other) noexcept;

    bool empty() const;
    size_t size() const;

    void clear();

    T& front();
    T& back();

    void push_back(const T& val);
    void push_front(const T& val);

    void pop_back();
    void pop_front();

    iterator insert(iterator pos, const T& val);
    iterator erase(iterator pos);

    iterator begin();
    iterator end();

    const_iterator begin() const;
    const_iterator end() const;
};

}

#include "list_impl.hpp"

#endif
