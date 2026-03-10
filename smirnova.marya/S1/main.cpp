#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <stdexcept>
#include <climits>

namespace smirnova {

// ------------------ Список ------------------
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
    const T& value() const { return node->data; }
};

// ------------------ Вспомогательные функции ------------------
template <typename T>
LCIter<T> getElementAt(const List<T>& list, size_t index) {
    LCIter<T> it = list.cbegin();
    size_t i = 0;
    while (it.valid() && i < index) { it.next(); ++i; }
    return it;
}

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

// ------------------ Основная логика ------------------
void processSequences(List<std::pair<std::string, List<int>>>& seq) {
    if (seq.empty()) return;

    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next())
        if (it.value().second.size() > maxSize) maxSize = it.value().second.size();

    if (maxSize == 0) maxSize = 1; // если все списки пустые, выводим хотя бы 0

    // вывод по строкам, пустые позиции заменяем на 0
    for (size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            int val = (row < it.value().second.size()) ? getElementAt(it.value().second, row).value() : 0;
            if (!firstInRow) std::cout << " ";
            std::cout << val;
            firstInRow = false;
        }
        std::cout << "\n";
    }

    // вычисление сумм с проверкой переполнения
    List<int> sums;
    for (size_t row = 0; row < maxSize; ++row) {
        int sum = 0;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            int val = (row < it.value().second.size()) ? getElementAt(it.value().second, row).value() : 0;
            if (val > 0 && sum > INT_MAX - val) {
                throw std::overflow_error("overflow");
            }
            sum += val;
        }
        sums.push_back(sum);
    }

    printList(sums);
}

// ------------------ main ------------------
int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    std::string line;
    bool overflowOccurred = false;

    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;

        std::istringstream iss(line);
        std::string name;
        iss >> name;

        List<int> numbers;
        unsigned long long temp;

        while (iss >> temp) {
            if (temp > static_cast<unsigned long long>(INT_MAX)) {
                overflowOccurred = true;
            } else {
                numbers.push_back(static_cast<int>(temp));
            }
        }

        sequences.push_back(std::make_pair(name, std::move(numbers)));
    }

    if (overflowOccurred) {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
    }

    if (sequences.empty()) {
        std::cout << "0\n";
        return 0;
    }

    // вывод названий
    bool first = true;
    for (LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value().first;
        first = false;
    }
    std::cout << "\n";

    try {
        processSequences(sequences);
    } catch (const std::overflow_error&) {
        std::cerr << "overflow\n";
        return 1;
    }

    return 0;
}
