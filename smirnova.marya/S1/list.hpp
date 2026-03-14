#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>
#include <limits>
#include <utility>
#include <stdexcept>
#include <cstddef>

namespace smirnova {

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

// --- Итераторы должны быть до List ---
template <typename T>
class LIter {
public:
    Node<T>* node;
    Node<T>* sentinel;
    LIter(Node<T>* n = nullptr, Node<T>* s = nullptr) : node(n), sentinel(s) {}
    bool valid() const { return node != sentinel; }
    void next() { if(node) node = node->next; }
    void prev() { if(node) node = node->prev; }
    T& value() { return node->data; }
};

template <typename T>
class LCIter {
public:
    const Node<T>* node;
    const Node<T>* sentinel;
    LCIter(const Node<T>* n = nullptr, const Node<T>* s = nullptr) : node(n), sentinel(s) {}
    LCIter(const LIter<T>& it) : node(it.node), sentinel(it.sentinel) {}
    bool valid() const { return node != sentinel; }
    void next() { if(node) node = node->next; }
    void prev() { if(node) node = node->prev; }
    const T& value() const { return node->data; }
};

// --- Теперь List<T> может использовать LIter<T> и LCIter<T> ---
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
        Node<T>* curr = sentinel->next;
        while(curr != sentinel) { Node<T>* tmp = curr->next; delete curr; curr = tmp; }
        sentinel->next = sentinel; sentinel->prev = sentinel; count = 0;
    }

    bool empty() const noexcept { return count == 0; }
    size_t size() const noexcept { return count; }

    void push_back(const T& val) { Node<T>* n = new Node<T>(val); n->prev = sentinel->prev; n->next = sentinel; sentinel->prev->next = n; sentinel->prev = n; ++count; }
    void push_front(const T& val) { Node<T>* n = new Node<T>(val); n->prev = sentinel; n->next = sentinel->next; sentinel->next->prev = n; sentinel->next = n; ++count; }

    void pop_back() { if(empty()) throw std::out_of_range("pop_back on empty"); Node<T>* tmp = sentinel->prev; sentinel->prev = tmp->prev; tmp->prev->next = sentinel; delete tmp; --count; }
    void pop_front() { if(empty()) throw std::out_of_range("pop_front on empty"); Node<T>* tmp = sentinel->next; sentinel->next = tmp->next; tmp->next->prev = sentinel; delete tmp; --count; }

    LIter<T> begin() { return LIter<T>(sentinel->next, sentinel); }
    LIter<T> end() { return LIter<T>(sentinel, sentinel); }
    LCIter<T> begin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> end() const { return LCIter<T>(sentinel, sentinel); }
    LCIter<T> cbegin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> cend() const { return LCIter<T>(sentinel, sentinel); }
};

template <typename T>
LCIter<T> getElementAt(const List<T>& lst, size_t idx) {
    LCIter<T> it = lst.cbegin();
    size_t i = 0;
    while(it.valid() && i < idx) { it.next(); ++i; }
    return it;
}

template <typename T>
void prsize_tList(const List<T>& lst) {
    if(lst.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for(LCIter<T> it = lst.cbegin(); it.valid(); it.next()) {
        if(!first) std::cout << " ";
        std::cout << it.value();
        first = false;
    }
    std::cout << "\n";
}

bool willOverflow(size_t a, size_t b) {
    return b > std::numeric_limits<size_t>::max() - a;
}

bool processSequences(const List<std::pair<std::string, List<size_t>>>& sequences) {
    // Проверка на пустой список
    if(sequences.empty()) {
        std::cout << "0\n";
        return true;
    }

    // Вывод названий
    bool firstName = true;
    for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if(!firstName) std::cout << " ";
        std::cout << it.value().first;
        firstName = false;
    }
    std::cout << "\n";

    // Определяем максимальную длину
    size_t maxLen = 0;
    for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if(it.value().second.size() > maxLen) maxLen = it.value().second.size();
    }

    // Печать строк с числами
    for(size_t row = 0; row < maxLen; ++row) {
        bool firstNum = true;
        for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                LCIter<size_t> numIt = getElementAt(it.value().second, row);
                if(!firstNum) std::cout << " ";
                std::cout << numIt.value();
                firstNum = false;
            }
        }
        if(!firstNum) std::cout << "\n";
    }

    // Вычисляем суммы
    List<size_t> sums;
    for(size_t row = 0; row < maxLen; ++row) {
        size_t sum = 0;
        for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                size_t val = getElementAt(it.value().second, row).value();
                if(willOverflow(sum, val)) {
                    std::cerr << "overflow\n";
                    return false;
                }
                sum += val;
            }
        }
        sums.push_back(sum);
    }

    prsize_tList(sums);
    return true;
}

} // namespace smirnova

#endif

