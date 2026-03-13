#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova {

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

template <typename T>
class LIter;

template <typename T>
class LCIter;

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

    void clear() noexcept {
        Node<T>* current = sentinel->next;
        while (current != sentinel) {
            Node<T>* tmp = current->next;
            delete current;
            current = tmp;
        }
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        count = 0;
    }

    ~List() {
        clear();
        delete sentinel;
    }

    // Копирование
    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    // Перемещение
    List(List&& other) noexcept : sentinel(other.sentinel), count(other.count) {
        other.sentinel = new Node<T>(T());
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.count = 0;
    }

    List& operator=(const List& other) {
        if (this != &other) {
            List tmp(other);
            swap(tmp);
        }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            delete sentinel;
            sentinel = other.sentinel;
            count = other.count;
            other.sentinel = new Node<T>(T());
            other.sentinel->next = other.sentinel;
            other.sentinel->prev = other.sentinel;
            other.count = 0;
        }
        return *this;
    }

    void swap(List& other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(count, other.count);
    }

    bool empty() const noexcept { return count == 0; }
    size_t size() const noexcept { return count; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++count;
    }

    void push_back(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++count;
    }

    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++count;
    }

    void push_front(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
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

template <typename T>
LCIter<T> getElementAt(const List<T>& lst, size_t idx) {
    LCIter<T> it = lst.cbegin();
    size_t i = 0;
    while(it.valid() && i < idx) { it.next(); ++i; }
    return it;
}

template <typename T>
void printList(const List<T>& lst) {
    if(lst.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for(LCIter<T> it = lst.cbegin(); it.valid(); it.next()) {
        if(!first) std::cout << " ";
        std::cout << it.value();
        first = false;
    }
    std::cout << "\n";
}

bool willOverflow(int a, int b) {
    if (b > 0 && a > std::numeric_limits<int>::max() - b) return true;
    if (b < 0 && a < std::numeric_limits<int>::min() - b) return true;
    return false;
}

bool processSequences(const List<std::pair<std::string, List<int>>>& sequences) {
    if(sequences.empty()) return true;

    // Вывод названий
    bool firstName = true;
    for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if(!firstName) std::cout << " ";
        std::cout << it.value().first;
        firstName = false;
    }
    std::cout << "\n";

    // Определяем максимальную длину
    size_t maxLen = 0;
    for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if(it.value().second.size() > maxLen) maxLen = it.value().second.size();
    }

    // Печать строк с числами
    for(size_t row = 0; row < maxLen; ++row) {
        bool firstNum = true;
        for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if(!firstNum) std::cout << " ";
                std::cout << numIt.value();
                firstNum = false;
            }
        }
        if(!firstNum) std::cout << "\n";
    }

    // Вычисляем суммы
    List<int> sums;
    for(size_t row = 0; row < maxLen; ++row) {
        int sum = 0;
        for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                int val = getElementAt(it.value().second, row).value();
                if(willOverflow(sum, val)) {
                    std::cout << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
                    return false;
                }
                sum += val;
            }
        }
        sums.push_back(sum);
    }

    printList(sums);
    return true;
}

} // namespace smirnova

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    bool overflowDetected = false;

    std::string line;
    while(std::getline(std::cin, line)) {
        if(line.empty()) continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        List<int> numbers;
        std::string token;
        while(iss >> token) {
            try {
                long long val = std::stoll(token);
                if(val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min()) {
                    overflowDetected = true;
                    break;
                }
                numbers.push_back(static_cast<int>(val));
            } catch(...) {
                overflowDetected = true;
                break;
            }
        }

        sequences.push_back({name, std::move(numbers)});
    }

    if(overflowDetected) {
        std::cout << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
    }

    if(!processSequences(sequences)) return 1;

    return 0;
}
