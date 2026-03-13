#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

namespace smirnova {

template <class T>
struct Node {
    T value;
    Node* next;
    Node* prev;
    Node(const T& v) : value(v), next(nullptr), prev(nullptr) {}
    Node(T&& v) : value(std::move(v)), next(nullptr), prev(nullptr) {}
};

template <class T> class LIter;
template <class T> class LCIter;

template <class T>
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

    bool empty() const noexcept { return count == 0; }
    size_t size() const noexcept { return count; }

    void push_back(const T& val) {
        Node<T>* node = new Node<T>(val);
        node->next = sentinel;
        node->prev = sentinel->prev;
        sentinel->prev->next = node;
        sentinel->prev = node;
        ++count;
    }

    void push_front(const T& val) {
        Node<T>* node = new Node<T>(val);
        node->prev = sentinel;
        node->next = sentinel->next;
        sentinel->next->prev = node;
        sentinel->next = node;
        ++count;
    }

    LIter<T> begin() { return LIter<T>(sentinel->next, sentinel); }
    LIter<T> end() { return LIter<T>(sentinel, sentinel); }
    LCIter<T> begin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> end() const { return LCIter<T>(sentinel, sentinel); }
    LCIter<T> cbegin() const { return LCIter<T>(sentinel->next, sentinel); }
    LCIter<T> cend() const { return LCIter<T>(sentinel, sentinel); }
};

template <class T>
class LCIter {
private:
    const Node<T>* current;
    const Node<T>* sentinel;
public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* s = nullptr) : current(n), sentinel(s) {}
    bool valid() const { return current != sentinel; }
    void next() { if(current) current = current->next; }
    const T& value() const { return current->value; }
};

template <typename T>
LCIter<T> getElementAt(const List<T>& lst, size_t index) {
    LCIter<T> iter = lst.cbegin();
    size_t i = 0;
    while(iter.valid() && i < index) { iter.next(); ++i; }
    return iter;
}

// Проверка переполнения size_t
size_t safeAdd(size_t x, size_t y) {
    const size_t max_val = std::numeric_limits<size_t>::max();
    if (max_val - x >= y) return x + y;
    throw std::overflow_error("overflow");
}

// Печать списка чисел
template <typename T>
void printList(const List<T>& lst) {
    if(lst.empty()) { std::cout << "0\n"; return; }
    bool first = true;
    for(LCIter<T> iter = lst.cbegin(); iter.valid(); iter.next()) {
        if(!first) std::cout << " ";
        std::cout << iter.value();
        first = false;
    }
    std::cout << "\n";
}

// Печать названий последовательностей
void printSequenceNames(const List<std::pair<std::string, List<size_t>>>& sequences) {
    bool first = true;
    for(LCIter<std::pair<std::string, List<size_t>>> iter = sequences.cbegin(); iter.valid(); iter.next()) {
        if(!first) std::cout << " ";
        std::cout << iter.value().first;
        first = false;
    }
    std::cout << "\n";
}

// Вычисление сумм с проверкой переполнения
void computeSums(const List<List<size_t>>& rows, List<size_t>& sums) {
    for(LCIter<List<size_t>> rowIt = rows.cbegin(); rowIt.valid(); rowIt.next()) {
        size_t total = 0;
        for(LCIter<size_t> colIt = (*rowIt).cbegin(); colIt.valid(); colIt.next()) {
            total = safeAdd(total, colIt.value());
        }
        sums.push_back(total);
    }
}

} // namespace smirnova

int main() {
    using namespace smirnova;

    List<std::pair<std::string, List<size_t>>> sequences;
    std::string inputLine;

    // Считывание данных
    while(std::getline(std::cin, inputLine)) {
        if(inputLine.empty()) continue;

        std::istringstream stream(inputLine);
        std::string seqName;
        stream >> seqName;

        List<size_t> numbers;
        size_t value;
        while(stream >> value) {
            numbers.push_back(value);
        }

        sequences.push_back({seqName, std::move(numbers)});
    }

    if(sequences.empty()) { std::cout << "0\n"; return 0; }

    // Вывод названий
    printSequenceNames(sequences);

    // Транспонирование строк
    List<List<size_t>> transposed;
    size_t maxLen = 0;
    for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next())
        if(it.value().second.size() > maxLen) maxLen = it.value().second.size();

    for(size_t row = 0; row < maxLen; ++row) {
        List<size_t> newRow;
        for(LCIter<std::pair<std::string, List<size_t>>> it = sequences.cbegin(); it.valid(); it.next()) {
            if(row < it.value().second.size()) {
                LCIter<size_t> cell = getElementAt(it.value().second, row);
                newRow.push_back(cell.value());
            }
        }
        transposed.push_back(newRow);
    }

    // Вывод транспонированных строк
    for(LCIter<List<size_t>> rowIt = transposed.cbegin(); rowIt.valid(); rowIt.next()) {
        printList(*rowIt);
    }

    // Суммы с проверкой переполнения
    try {
        List<size_t> sums;
        computeSums(transposed, sums);
        printList(sums);
    } catch(const std::overflow_error& e) {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
    }

    return 0;
}
