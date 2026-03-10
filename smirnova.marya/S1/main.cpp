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

    void clear() noexcept {              // <-- clear объявлен до деструктора
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

    ~List() { clear(); delete fake; }

    void push_back(const T& val) {
        Node<T>* n = new Node<T>(val);
        n->next = fake;
        n->prev = fake->prev;
        fake->prev->next = n;
        fake->prev = n;
        ++sz;
    }

    bool empty() const noexcept { return sz == 0; }
    size_t size() const noexcept { return sz; }

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
    void next() { if(node) node = node->next; }
    T& value() { return node->data; }
};

template <class T>
class LCIter {
private:
    const Node<T>* node;
    const Node<T>* fake;
public:
    LCIter(const Node<T>* n = nullptr, const Node<T>* f = nullptr) : node(n), fake(f) {}
    bool valid() const { return node != fake; }
    void next() { if(node) node = node->next; }
    const T& value() const { return node->data; }
};

template <typename T>
LCIter<T> getElementAt(const List<T>& list, size_t index) {
    LCIter<T> it = list.cbegin();
    size_t i = 0;
    while (it.valid() && i < index) { it.next(); ++i; }
    return it;
}

void processSequences(List<std::pair<std::string, List<int>>>& seq) {
    if (seq.empty()) return;

    // Вычисление максимальной длины
    size_t maxSize = 0;
    for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next())
        if (it.value().second.size() > maxSize) maxSize = it.value().second.size();

    if (maxSize == 0) { std::cout << "0\n"; return; }

    // Вывод по строкам
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

    // Вычисление суммы по строкам
    for (size_t row = 0; row < maxSize; ++row) {
        int sum = 0;
        for (LCIter<std::pair<std::string, List<int>>> it = seq.cbegin(); it.valid(); it.next()) {
            if (row < it.value().second.size()) {
                LCIter<int> numIt = getElementAt(it.value().second, row);
                if (numIt.value() > 0 && sum > INT_MAX - numIt.value()) {
                    throw std::overflow_error("overflow");
                }
                sum += numIt.value();
            }
        }
    }
}

} // namespace smirnova

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

    // Вывод названий
    bool first = true;
    for (LCIter<std::pair<std::string, List<int>>> it = sequences.cbegin(); it.valid(); it.next()) {
        if (!first) std::cout << " ";
        std::cout << it.value().first;
        first = false;
    }
    std::cout << "\n";

    // Обработка сумм с try/catch на переполнение
    try {
        processSequences(sequences);
    } catch (const std::overflow_error&) {
        std::cerr << "Formed lists with exit code 1 and error message in standard error because of overflow\n";
        return 1;
    }

    return 0;
}
