#include <iostream>
#include <sstream>
#include <string>
#include <limits>
#include <stdexcept>
#include <utility>

namespace smirnova {

template <typename T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    Node(T&& val) : data(std::move(val)), next(nullptr), prev(nullptr) {}
};

template <typename T> class LCIter;

template <typename T>
class List {
private:
    Node<T>* sentinel;
    size_t sz;

public:
    List() : sz(0) {
        sentinel = new Node<T>(T());
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }

    ~List() {
        clear();
        delete sentinel;
    }

    void clear() {
        Node<T>* cur = sentinel->next;
        while (cur != sentinel) {
            Node<T>* tmp = cur->next;
            delete cur;
            cur = tmp;
        }
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        sz = 0;
    }

    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    List(List&& other) noexcept : sentinel(other.sentinel), sz(other.sz) {
        other.sentinel = new Node<T>(T());
        other.sentinel->next = other.sentinel->prev = other.sentinel;
        other.sz = 0;
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
            sz = other.sz;
            other.sentinel = new Node<T>(T());
            other.sentinel->next = other.sentinel->prev = other.sentinel;
            other.sz = 0;
        }
        return *this;
    }

    void swap(List& other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(sz, other.sz);
    }

    bool empty() const { return sz == 0; }
    size_t size() const { return sz; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++sz;
    }

    void push_back(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++sz;
    }

    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++sz;
    }

    void push_front(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++sz;
    }

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
    void next() { node = node->next; }
    const T& value() const { return node->data; }
};

// Получение элемента по индексу
template <typename T>
LCIter<T> getElementAt(const List<T>& lst, size_t idx) {
    LCIter<T> it = lst.cbegin();
    size_t i = 0;
    while (it.valid() && i < idx) { it.next(); ++i; }
    return it;
}

// Вывод чисел из списка без лишнего пробела
template <typename T>
void printList(const List<T>& lst) {
    if (lst.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for (LCIter<T> it = lst.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << ' ';
        std::cout << it.value();
        first = false;
    }
    std::cout << '\n';
}

// Проверка переполнения при сумме
bool sumWillOverflow(int a, int b) {
    if (b > 0 && a > std::numeric_limits<int>::max() - b) return true;
    if (b < 0 && a < std::numeric_limits<int>::min() - b) return true;
    return false;
}

// Обработка последовательностей чисел
bool processSequences(const List<std::pair<std::string, List<int>>>& seq) {
    if (seq.empty()) return true;

    // Печать названий
    bool firstName = true;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
        if (!firstName) std::cout << ' ';
        std::cout << it.value().first;
        firstName = false;
    }
    std::cout << '\n';

    // Определяем максимальный размер столбцов
    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
        if (it.value().second.size() > maxSize) maxSize = it.value().second.size();
    }

    // Вывод чисел по строкам
    for (size_t row = 0; row < maxSize; ++row) {
        bool firstInRow = true;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (!firstInRow) std::cout << ' ';
                std::cout << numIt.value();
                firstInRow = false;
            }
        }
        if (!firstInRow) std::cout << '\n';
    }

    // Вычисляем суммы по строкам с проверкой переполнения
    List<int> sums;
    for (size_t row = 0; row < maxSize; ++row) {
        int sum = 0;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                int val = getElementAt(it.value().second, row).value();
                if (sumWillOverflow(sum, val)) {
                    std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
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
    bool overflowOccurred = false;

    std::string line;
    while (std::getline(std::cin, line)) {
        if (line.empty()) continue;
        std::istringstream iss(line);
        std::string name;
        iss >> name;

        List<int> numbers;
        std::string token;
        while (iss >> token) {
            try {
                long long val = std::stoll(token);
                if (val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min()) {
                    overflowOccurred = true;
                    break;
                }
                numbers.push_back(static_cast<int>(val));
            } catch (...) {
                overflowOccurred = true;
                break;
            }
        }

        sequences.push_back({name, std::move(numbers)});
    }

    if (overflowOccurred) {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
    }

    if (!processSequences(sequences)) return 1;

    // Пустой список → 0
    if (sequences.empty()) std::cout << "0\n";

    return 0;
}
