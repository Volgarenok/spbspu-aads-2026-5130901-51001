#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <climits>

namespace smirnova {

template <class T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& d) : data(d), next(nullptr), prev(nullptr) {}
    Node(T&& d) : data(std::move(d)), next(nullptr), prev(nullptr) {}
};

template <class T> class LIter;
template <class T> class LCIter;

template <class T>
class List {
private:
    Node<T>* fake;
    size_t sz;

public:
    List() : sz(0) {
        fake = new Node<T>(T());
        fake->next = fake;
        fake->prev = fake;
    }
    ~List() { clear(); delete fake; }

    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    List(List&& other) noexcept : fake(other.fake), sz(other.sz) {
        other.fake = new Node<T>(T());
        other.fake->next = other.fake;
        other.fake->prev = other.fake;
        other.sz = 0;
    }

    List& operator=(const List& other) {
        if (this != &other) { List tmp(other); swap(tmp); }
        return *this;
    }

    List& operator=(List&& other) noexcept {
        if (this != &other) {
            clear();
            delete fake;
            fake = other.fake;
            sz = other.sz;
            other.fake = new Node<T>(T());
            other.fake->next = other.fake;
            other.fake->prev = other.fake;
            other.sz = 0;
        }
        return *this;
    }

    void swap(List& other) noexcept {
        std::swap(fake, other.fake);
        std::swap(sz, other.sz);
    }
    bool empty() const noexcept { return sz == 0; }
    size_t size() const noexcept { return sz; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = fake;
        n->prev = fake->prev;
        fake->prev->next = n;
        fake->prev = n;
        ++sz;
    }
    void push_back(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->next = fake;
        n->prev = fake->prev;
        fake->prev->next = n;
        fake->prev = n;
        ++sz;
    }
    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = fake;
        n->next = fake->next;
        fake->next->prev = n;
        fake->next = n;
        ++sz;
    }
    void push_front(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->prev = fake;
        n->next = fake->next;
        fake->next->prev = n;
        fake->next = n;
        ++sz;
    }
    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty list");
        Node<T>* tmp = fake->next;
        fake->next = tmp->next;
        tmp->next->prev = fake;
        delete tmp;
        --sz;
    }
    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty list");
        Node<T>* tail = fake->prev;
        fake->prev = tail->prev;
        tail->prev->next = fake;
        delete tail;
        --sz;
    }
    void clear() noexcept {
        Node<T>* curr = fake->next;
        while (curr != fake) {
            Node<T>* tmp = curr->next;
            delete curr;
            curr = tmp;
        }
        fake->next = fake;
        fake->prev = fake;
        sz = 0;
    }

    LIter<T> begin() { return LIter<T>(fake->next, fake); }
    LIter<T> end() { return LIter<T>(fake, fake); }
    LCIter<T> begin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> end() const { return LCIter<T>(fake, fake); }
    LCIter<T> cbegin() const { return LCIter<T>(fake->next, fake); }
    LCIter<T> cend() const { return LCIter<T>(fake, fake); }
};

template <class T>
class LIter {
private:
    Node<T>* node;
    Node<T>* fake;

public:
    LIter(Node<T>* n = nullptr, Node<T>* f = nullptr) : node(n), fake(f) {}
    bool valid() const { return node != fake; }
    void next() { if (node) node = node->next; }
    void prev() { if (node) node = node->prev; }
    T& value() { return node->data; }
};

template <class T>
class LCIter {
private:
    const Node<T>* node;
    const Node<T>* fake;

public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* f = nullptr) : node(n), fake(f) {}
    LCIter(const LIter<T>& iter) : node(iter.node), fake(iter.fake) {}
    bool valid() const { return node != fake; }
    void next() { if (node) node = node->next; }
    void prev() { if (node) node = node->prev; }
    const T& value() const { return node->data; }
};

template <typename T>
void printList(const List<T>& list) {
    if (list.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for (LCIter<T> it = list.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value();
        first = false;
    }
    std::cout << "\n";
}

template <typename T>
LCIter<T> getElementAt(const List<T>& list, size_t index) {
    LCIter<T> it = list.cbegin();
    size_t i = 0;
    while (it.valid() && i < index) { it.next(); ++i; }
    return it;
}

void processSequences(List<std::pair<std::string, List<int>>>& seq) {
    if (seq.empty()) return;

    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next())
        if (it.value().second.size() > maxSize) maxSize = it.value().second.size();

    if (maxSize == 0) { std::cout << "0\n"; return; }

    for (size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (!firstInRow) std::cout << " ";
                std::cout << numIt.value();
                firstInRow = false;
            }
        }
        if (!firstInRow) std::cout << "\n";
    }

    List<int> sums;
    for (size_t row = 0; row < maxSize; ++row) {
        int sum = 0;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (numIt.value() > 0 && sum > INT_MAX - numIt.value()) {
                    std::cerr << "number exceeds int range\n";
                    exit(1); // прерывание программы с ошибкой
                }
                sum += numIt.value();
            }
        }
        sums.push_back(sum);
    }

    printList(sums);
}

} // namespace smirnova

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    std::string line;
    bool overflowOccurred = false; // флаг переполнения

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        List<int> numbers;
        unsigned long long temp;

        while (iss >> temp) {
            if (temp > static_cast<unsigned long long>(INT_MAX)) {
                overflowOccurred = true; // помечаем переполнение
            } else {
                numbers.push_back(static_cast<int>(temp));
            }
        }

        sequences.push_back(std::make_pair(name, std::move(numbers)));
    }

    // Проверка переполнения после всего ввода
    if (overflowOccurred) {
        std::cerr << "number exceeds int range\n";
        return 1;
    }

    if (sequences.empty()) {
        std::cout << "0\n";
        return 0;
    }

    // Вывод названий
    bool first = true;
    for (LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value().first;
        first = false;
    }
    std::cout << "\n";

    processSequences(sequences);

    return 0;
}

