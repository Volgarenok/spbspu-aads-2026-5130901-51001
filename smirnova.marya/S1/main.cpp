#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova {

// --- Node ---
template <class T>
struct Node {
    T data;
    Node* next;
    Node* prev;
    Node(const T& val) : data(val), next(nullptr), prev(nullptr) {}
    Node(T&& val) : data(std::move(val)), next(nullptr), prev(nullptr) {}
};

// --- Forward declarations ---
template <class T> class LIter;
template <class T> class LCIter;

// --- List ---
template <class T>
class List {
private:
    Node<T>* sentinel;
    size_t length;

public:
    List() : length(0) {
        sentinel = new Node<T>(T());
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
    }

    ~List() { clear(); delete sentinel; }

    List(const List& other) : List() {
        for (LCIter<T> it = other.cbegin(); it.valid(); it.next())
            push_back(it.value());
    }

    List(List&& other) noexcept : sentinel(other.sentinel), length(other.length) {
        other.sentinel = new Node<T>(T());
        other.sentinel->next = other.sentinel;
        other.sentinel->prev = other.sentinel;
        other.length = 0;
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
            length = other.length;
            other.sentinel = new Node<T>(T());
            other.sentinel->next = other.sentinel;
            other.sentinel->prev = other.sentinel;
            other.length = 0;
        }
        return *this;
    }

    void swap(List& other) noexcept {
        std::swap(sentinel, other.sentinel);
        std::swap(length, other.length);
    }

    bool empty() const noexcept { return length == 0; }
    size_t size() const noexcept { return length; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++length;
    }

    void push_back(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->next = sentinel;
        n->prev = sentinel->prev;
        sentinel->prev->next = n;
        sentinel->prev = n;
        ++length;
    }

    void push_front(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++length;
    }

    void push_front(T&& val) {
        Node<T>* n = new Node<T>(std::move(val));
        n->prev = sentinel;
        n->next = sentinel->next;
        sentinel->next->prev = n;
        sentinel->next = n;
        ++length;
    }

    void pop_front() {
        if (empty()) throw std::out_of_range("pop_front on empty list");
        Node<T>* tmp = sentinel->next;
        sentinel->next = tmp->next;
        tmp->next->prev = sentinel;
        delete tmp;
        --length;
    }

    void pop_back() {
        if (empty()) throw std::out_of_range("pop_back on empty list");
        Node<T>* tmp = sentinel->prev;
        sentinel->prev = tmp->prev;
        tmp->prev->next = sentinel;
        delete tmp;
        --length;
    }

    // Итераторы
    LIter<T> begin() { return LIter<T>(sentinel->next, sentinel); }
    LIter<T> end() { return LIter<T>(sentinel, sentinel); }
    LCIter<T> begin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> end() const { return LCIter<T>(sentinel, sentinel); }
    LCIter<T> cbegin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> cend() const { return LCIter<T>(sentinel, sentinel); }
};

// --- Const Iterator ---
template <class T>
class LCIter {
private:
    const Node<T>* current;
    const Node<T>* sentinel;
public:
    LCIter(const Node<T>* node = nullptr, const Node<T>* endNode = nullptr)
        : current(node), sentinel(endNode) {}

    bool valid() const { return current != sentinel; }
    void next() { if(current) current = current->next; }
    const T& value() const { return current->data; }

    // Добавлен operator* для удобного доступа
    const T& operator*() const { return current->data; }
};

// --- Получение элемента по индексу ---
template <typename T>
LCIter<T> getElementAt(const List<T>& lst, size_t idx) {
    LCIter<T> it = lst.cbegin();
    size_t counter = 0;
    while(it.valid() && counter < idx) { it.next(); ++counter; }
    return it;
}

// --- Печать списка ---
template <typename T>
void printList(const List<T>& lst) {
    if(lst.empty()) { std::cout << "0\n"; return; }
    bool firstElem = true;
    for(LCIter<T> it = lst.cbegin(); it.valid(); it.next()) {
        if(!firstElem) std::cout << " ";
        std::cout << it.value();
        firstElem = false;
    }
    std::cout << "\n";
}

// --- Проверка переполнения ---
bool willOverflow(int a, int b) {
    if (b > 0 && a > INT_MAX - b) return true;
    if (b < 0 && a < INT_MIN - b) return true;
    return false;
}

// --- Обработка последовательностей ---
bool processSequences(const List<std::pair<std::string, List<int>>>& sequences) {
    if(sequences.empty()) return true;

    size_t maxRowSize = 0;
    for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next())
        if(it.value().second.size() > maxRowSize) maxRowSize = it.value().second.size();

    // Вывод названий строк
    for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next())
        std::cout << it.value().first << " ";
    std::cout << "\n";

    // Вывод чисел по строкам
    for(size_t row = 0; row < maxRowSize; ++row) {
        bool firstInRow = true;
        for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if(!firstInRow) std::cout << " ";
                std::cout << *numIt;
                firstInRow = false;
            }
        }
        if(!firstInRow) std::cout << "\n";
    }

    // Суммы с проверкой переполнения
    List<int> sums;
    for(size_t row = 0; row < maxRowSize; ++row) {
        int sum = 0;
        for(LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                int val = getElementAt(it.value().second, row).value();
                if((val > 0 && sum > INT_MAX - val) ||
                   (val < 0 && sum < INT_MIN - val)) {
                    std::cerr << "Overflow detected\n";
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

// --- Main ---
int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<int>>> sequences;
    std::string inputLine;
    bool overflowFlag = false;

    while(std::getline(std::cin, inputLine)) {
        if(inputLine.empty()) continue;

        std::istringstream iss(inputLine);
        std::string label;
        iss >> label;

        List<int> numbers;
        std::string token;
        while(iss >> token) {
            try {
                long long val = std::stoll(token);
                if(val > std::numeric_limits<int>::max() || val < std::numeric_limits<int>::min()) {
                    overflowFlag = true;
                    break;
                }
                numbers.push_back(static_cast<int>(val));
            } catch(...) {
                overflowFlag = true;
                break;
            }
        }
        sequences.push_back({label, std::move(numbers)});
    }

    if(overflowFlag) {
        std::cerr << "Overflow in input numbers\n";
        return 1;
    }

    if(!processSequences(sequences)) return 1;

    return 0;
}
