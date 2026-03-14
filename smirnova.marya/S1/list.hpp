#ifndef LIST_HPP
#define LIST_HPP

#include <cstddef>
#include <stdexcept>
#include <utility>

namespace smirnova {

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

template <typename T> class LIter;
template <typename T> class LCIter;

template <typename T>
class List {
private:
    Node<T>* sentinel;
    size_t count;

public:
    List() : count(0) {
        sentinel = new Node<T>(T());
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }

    ~List() { clear(); delete sentinel; }

    void clear() noexcept {
        Node<T>* cur = sentinel->next;
        while (cur != sentinel) {
            Node<T>* tmp = cur->next;
            delete cur;
            cur = tmp;
        }
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        count = 0;
    }

    bool empty() const noexcept { return count == 0; }
    size_t size() const noexcept { return count; }

    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++count;
    }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++count;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty list");
        Node<T>* tmp = sentinel->next;
        sentinel->next = tmp->next;
        tmp->next->prev = sentinel;
        delete tmp;
        --count;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty list");
        Node<T>* tmp = sentinel->prev;
        sentinel->prev = tmp->prev;
        tmp->prev->next = sentinel;
        delete tmp;
        --count;
    }

    LIter<T> begin() { return LIter<T>(sentinel->next, sentinel); }
    LIter<T> end() { return LIter<T>(sentinel, sentinel); }
    LCIter<T> begin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> end() const { return LCIter<T>(sentinel, sentinel); }
    LCIter<T> cbegin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> cend() const { return LCIter<T>(sentinel, sentinel); }
};

template <typename T>
class LIter {
private:
    Node<T>* node;
    Node<T>* sentinel;
public:
    LIter(Node<T>* n = nullptr, Node<T>* s = nullptr) : node(n), sentinel(s) {}
    bool valid() const { return node != sentinel; }
    void next() { if (node) node = node->next; }
    void prev() { if (node) node = node->prev; }
    T& value() { return node->data; }
};

template <typename T>
class LCIter {
private:
    const Node<T>* node;
    const Node<T>* sentinel;
public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* s = nullptr) : node(n), sentinel(s) {}
    bool valid() const { return node != sentinel; }
    void next() { if(node) node = node->next; }
    const T& value() const { return node->data; }
};

} // namespace smirnova

#endif // LIST_HPP

